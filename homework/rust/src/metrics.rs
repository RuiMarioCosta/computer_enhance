// Port of metrics.hpp / metrics.cpp
//
// The CPU timer is read with RDTSC. For the OS-level reference clock used to
// estimate the CPU frequency, we use std::time::Instant so the code stays
// dependency-free and cross-platform (the C++ version uses
// QueryPerformanceCounter on Windows / gettimeofday elsewhere).

use std::time::{Duration, Instant};

#[inline]
pub fn read_cpu_timer() -> u64 {
    // NOTE(casey): If you were on ARM, you would need to replace this with one
    // of their performance counter read instructions.
    #[cfg(target_arch = "x86_64")]
    unsafe {
        core::arch::x86_64::_rdtsc()
    }
    #[cfg(target_arch = "x86")]
    unsafe {
        core::arch::x86::_rdtsc()
    }
    #[cfg(not(any(target_arch = "x86_64", target_arch = "x86")))]
    {
        // Fallback for non-x86 targets.
        Instant::now().elapsed().as_nanos() as u64
    }
}

pub fn get_cpu_freq() -> u64 {
    let milliseconds_to_wait = 100;
    let wait = Duration::from_millis(milliseconds_to_wait);

    let cpu_start = read_cpu_timer();
    let os_start = Instant::now();
    while os_start.elapsed() < wait {}
    let cpu_elapsed = read_cpu_timer() - cpu_start;

    let os_elapsed = os_start.elapsed().as_secs_f64();
    if os_elapsed > 0.0 {
        (cpu_elapsed as f64 / os_elapsed) as u64
    } else {
        0
    }
}
