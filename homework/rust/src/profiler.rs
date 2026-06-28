// Port of profiler.hpp / profiler.cpp
//
// The C++ version uses RAII (a destructor) plus __COUNTER__ to give every
// profiling call-site a unique anchor index. In Rust we mirror this with:
//   * a `Drop` impl on `ProfileScope` (RAII),
//   * a per-call-site `static AtomicU32` that lazily claims a global anchor
//     index the first time it runs (the equivalent of __COUNTER__).
//
// Global state lives in a single `UnsafeCell` behind a `static`, mirroring the
// single-threaded global profiler in the C++ code.

use crate::metrics::{get_cpu_freq, read_cpu_timer};
use std::cell::UnsafeCell;
use std::sync::atomic::{AtomicU32, Ordering};

pub const GLOBAL_PROFILER_ANCHOR_COUNT: usize = 4096;

#[derive(Clone, Copy)]
pub struct ProfileAnchor {
    pub tsc_elapsed_exclusive: u64, // does not include children
    pub tsc_elapsed_inclusive: u64, // includes children
    pub hit_count: u64,
    pub processed_byte_count: u64,
    pub label: &'static str,
}

impl ProfileAnchor {
    const ZERO: ProfileAnchor = ProfileAnchor {
        tsc_elapsed_exclusive: 0,
        tsc_elapsed_inclusive: 0,
        hit_count: 0,
        processed_byte_count: 0,
        label: "",
    };
}

struct ProfilerState {
    anchors: [ProfileAnchor; GLOBAL_PROFILER_ANCHOR_COUNT],
    parent: usize,
    start: u64,
    end: u64,
}

struct GlobalProfiler(UnsafeCell<ProfilerState>);
// SAFETY: this profiler is intended for single-threaded use, exactly like the
// global state in the original C++ implementation.
unsafe impl Sync for GlobalProfiler {}

static GLOBAL: GlobalProfiler = GlobalProfiler(UnsafeCell::new(ProfilerState {
    anchors: [ProfileAnchor::ZERO; GLOBAL_PROFILER_ANCHOR_COUNT],
    parent: 0,
    start: 0,
    end: 0,
}));

// Anchor index 0 is reserved as the "dummy parent" (mirrors __COUNTER__ + 1).
static GLOBAL_ANCHOR_COUNT: AtomicU32 = AtomicU32::new(1);

#[inline]
fn state() -> *mut ProfilerState {
    GLOBAL.0.get()
}

/// Lazily claim a unique anchor index for a call-site (the __COUNTER__ analog).
pub fn get_anchor_index(slot: &AtomicU32) -> usize {
    let current = slot.load(Ordering::Relaxed);
    if current != 0 {
        return current as usize;
    }
    let new_index = GLOBAL_ANCHOR_COUNT.fetch_add(1, Ordering::Relaxed);
    assert!(
        (new_index as usize) < GLOBAL_PROFILER_ANCHOR_COUNT,
        "Number of profile points exceeds size of profiler anchors array"
    );
    slot.store(new_index, Ordering::Relaxed);
    new_index as usize
}

pub struct ProfileScope {
    label: &'static str,
    index: usize,
    parent_index: usize,
    old_tsc_elapsed_inclusive: u64,
    start: u64,
}

impl ProfileScope {
    pub fn new(label: &'static str, index: usize, byte_count: u64) -> Self {
        unsafe {
            let s = state();
            let parent_index = (*s).parent;
            let old_tsc_elapsed_inclusive = (*s).anchors[index].tsc_elapsed_inclusive;
            (*s).parent = index;
            (*s).anchors[index].processed_byte_count += byte_count;
            ProfileScope {
                label,
                index,
                parent_index,
                old_tsc_elapsed_inclusive,
                start: read_cpu_timer(),
            }
        }
    }
}

impl Drop for ProfileScope {
    fn drop(&mut self) {
        let elapsed = read_cpu_timer() - self.start;
        unsafe {
            let s = state();
            let anchor = &mut (*s).anchors[self.index];
            anchor.tsc_elapsed_inclusive = self.old_tsc_elapsed_inclusive + elapsed;
            anchor.tsc_elapsed_exclusive = anchor.tsc_elapsed_exclusive.wrapping_add(elapsed);
            anchor.hit_count += 1;
            anchor.label = self.label;

            let parent = &mut (*s).anchors[self.parent_index];
            parent.tsc_elapsed_exclusive = parent.tsc_elapsed_exclusive.wrapping_sub(elapsed);

            (*s).parent = self.parent_index;
        }
    }
}

/// Returns the (short) name of the enclosing function, mirroring `__func__`.
#[macro_export]
macro_rules! function_name {
    () => {{
        fn f() {}
        fn type_name_of<T>(_: T) -> &'static str {
            std::any::type_name::<T>()
        }
        let name = type_name_of(f);
        // Strip the trailing "::f".
        let name = &name[..name.len() - 3];
        match name.rfind(':') {
            Some(pos) => &name[pos + 1..],
            None => name,
        }
    }};
}

/// Time a scope and attribute a processed-byte count to it (for bandwidth).
#[macro_export]
macro_rules! time_bandwidth {
    ($name:expr, $bytes:expr) => {
        // Each macro expansion gets its own block scope, so the inner `static`
        // never collides with another expansion in the same function.
        let _profile_block = {
            static ANCHOR_INDEX: std::sync::atomic::AtomicU32 =
                std::sync::atomic::AtomicU32::new(0);
            let idx = $crate::profiler::get_anchor_index(&ANCHOR_INDEX);
            $crate::profiler::ProfileScope::new($name, idx, $bytes as u64)
        };
    };
}

/// Time a scope by name.
#[macro_export]
macro_rules! time_block {
    ($name:expr) => {
        $crate::time_bandwidth!($name, 0u64);
    };
}

/// Time the enclosing function.
#[macro_export]
macro_rules! time_function {
    () => {
        $crate::time_block!($crate::function_name!());
    };
}

pub fn begin_profile() {
    unsafe {
        (*state()).start = read_cpu_timer();
    }
}

fn print_profile(total_elapsed: u64, cpu_freq: u64, anchor: &ProfileAnchor) {
    let elapsed = anchor.tsc_elapsed_exclusive;
    let percent = 100.0 * (elapsed as f64 / total_elapsed as f64);

    let mut line = format!(
        "  {}[{}]: {} ({:.2}%)",
        anchor.label, anchor.hit_count, anchor.tsc_elapsed_exclusive, percent
    );

    if elapsed != anchor.tsc_elapsed_inclusive {
        let percent_with_children =
            100.0 * (anchor.tsc_elapsed_inclusive as f64 / total_elapsed as f64);
        line += &format!(" {:.2}% w/children", percent_with_children);
    }

    line += ")";

    if anchor.processed_byte_count != 0 {
        let mebibyte = 1024.0 * 1024.0;
        let gibibyte = mebibyte * 1024.0;
        let seconds = anchor.tsc_elapsed_inclusive as f64 / cpu_freq as f64;
        let bytes_per_second = anchor.processed_byte_count as f64 / seconds;
        let mebibytes = anchor.processed_byte_count as f64 / mebibyte;
        let gibibytes_per_second = bytes_per_second / gibibyte;
        line += &format!(" {:.3}mb at {:.2}gb/s", mebibytes, gibibytes_per_second);
    }

    println!("{}", line);
}

fn print_anchor_data(total_elapsed: u64, cpu_freq: u64) {
    unsafe {
        let s = state();
        for anchor in (*s).anchors.iter() {
            if anchor.tsc_elapsed_inclusive != 0 {
                print_profile(total_elapsed, cpu_freq, anchor);
            }
        }
    }
}

pub fn end_and_print_profile() {
    unsafe {
        let s = state();
        (*s).end = read_cpu_timer();

        let cpu_freq = get_cpu_freq();
        let total_cpu_elapsed = (*s).end - (*s).start;

        if cpu_freq != 0 {
            println!(
                "\nTotal time: {:.4}ms (CPU freq {})",
                1000.0 * total_cpu_elapsed as f64 / cpu_freq as f64,
                cpu_freq
            );
        }
        print_anchor_data(total_cpu_elapsed, cpu_freq);
    }
}
