// Port of listing_0102_read_overhead_test.cpp
//
// Three different ways to read an entire file into a pre-allocated buffer, each
// wrapped so the repetition tester can measure the read overhead:
//
//   * `read_via_fread`     - the C stdio `fread` (cross-platform via the CRT)
//   * `read_via_read`      - the low-level `_read` / `read` syscall wrapper
//   * `read_via_read_file` - the Win32 `ReadFile` API (Windows only)
//
// The original is Windows-specific (it includes <windows.h>). We keep the same
// three functions, gating the Win32-only one behind `cfg(windows)` and using a
// POSIX fallback for the low-level reader on other platforms. All foreign
// functions are declared inline so the crate stays dependency-free.

use crate::repetition_tester::RepetitionTester;
use std::ffi::CString;
use std::os::raw::{c_char, c_int, c_void};

pub type ReadOverheadTestFunc = fn(&mut RepetitionTester, &mut [u8], &str);

// --- C stdio (fread) --------------------------------------------------------

extern "C" {
    fn fopen(filename: *const c_char, mode: *const c_char) -> *mut c_void;
    fn fread(ptr: *mut c_void, size: usize, nmemb: usize, stream: *mut c_void) -> usize;
    fn fclose(stream: *mut c_void) -> c_int;
}

pub fn read_via_fread(tester: &mut RepetitionTester, dest: &mut [u8], file_name: &str) {
    let mode = CString::new("rb").unwrap();
    while tester.is_testing() {
        let name = match CString::new(file_name) {
            Ok(name) => name,
            Err(_) => {
                tester.error("invalid file name");
                break;
            }
        };

        let file = unsafe { fopen(name.as_ptr(), mode.as_ptr()) };
        if !file.is_null() {
            let count = dest.len();

            tester.begin_time();
            let result = unsafe { fread(dest.as_mut_ptr() as *mut c_void, count, 1, file) };
            tester.end_time();

            if result == 1 {
                tester.count_bytes(count as u64);
            } else {
                tester.error("fread failed");
            }

            unsafe {
                fclose(file);
            }
        } else {
            tester.error("fopen failed");
        }
    }
}

// --- Low-level read (_read / read) ------------------------------------------

#[cfg(windows)]
mod lowlevel {
    use std::os::raw::{c_char, c_int, c_uint, c_void};

    // _O_RDONLY (0x0000) | _O_BINARY (0x8000)
    pub const OPEN_FLAGS: c_int = 0x0000 | 0x8000;
    pub const MAX_READ: c_uint = c_int::MAX as c_uint;

    extern "C" {
        #[link_name = "_open"]
        pub fn open(filename: *const c_char, oflag: c_int) -> c_int;
        #[link_name = "_read"]
        pub fn read(fd: c_int, buf: *mut c_void, count: c_uint) -> c_int;
        #[link_name = "_close"]
        pub fn close(fd: c_int) -> c_int;
    }
}

#[cfg(not(windows))]
mod lowlevel {
    use std::os::raw::{c_char, c_int, c_uint, c_void};

    // O_RDONLY is 0 on Linux/macOS; there is no binary mode flag.
    pub const OPEN_FLAGS: c_int = 0;
    pub const MAX_READ: c_uint = c_int::MAX as c_uint;

    extern "C" {
        pub fn open(filename: *const c_char, oflag: c_int) -> c_int;
        pub fn read(fd: c_int, buf: *mut c_void, count: usize) -> isize;
        pub fn close(fd: c_int) -> c_int;
    }
}

pub fn read_via_read(tester: &mut RepetitionTester, dest: &mut [u8], file_name: &str) {
    while tester.is_testing() {
        let name = match CString::new(file_name) {
            Ok(name) => name,
            Err(_) => {
                tester.error("invalid file name");
                break;
            }
        };

        let file = unsafe { lowlevel::open(name.as_ptr(), lowlevel::OPEN_FLAGS) };
        if file != -1 {
            let mut offset = 0usize;
            let mut size_remaining = dest.len() as u64;

            while size_remaining != 0 {
                let mut read_size = lowlevel::MAX_READ;
                if read_size as u64 > size_remaining {
                    read_size = size_remaining as c_int as _;
                }

                let ptr = unsafe { dest.as_mut_ptr().add(offset) } as *mut c_void;

                tester.begin_time();
                let result = unsafe { lowlevel::read(file, ptr, read_size as _) };
                tester.end_time();

                if result as i64 == read_size as i64 {
                    tester.count_bytes(read_size as u64);
                } else {
                    tester.error("_read failed");
                    break;
                }

                size_remaining -= read_size as u64;
                offset += read_size as usize;
            }

            unsafe {
                lowlevel::close(file);
            }
        } else {
            tester.error("_open failed");
        }
    }
}

// --- Win32 ReadFile ---------------------------------------------------------

#[cfg(windows)]
mod win32 {
    use std::os::raw::{c_char, c_void};

    pub type Handle = *mut c_void;
    pub type Dword = u32;
    pub type Bool = i32;

    pub const GENERIC_READ: Dword = 0x8000_0000;
    pub const FILE_SHARE_READ: Dword = 0x0000_0001;
    pub const FILE_SHARE_WRITE: Dword = 0x0000_0002;
    pub const OPEN_EXISTING: Dword = 3;
    pub const FILE_ATTRIBUTE_NORMAL: Dword = 0x0000_0080;

    pub fn invalid_handle_value() -> Handle {
        -1isize as Handle
    }

    extern "system" {
        pub fn CreateFileA(
            file_name: *const c_char,
            desired_access: Dword,
            share_mode: Dword,
            security_attributes: *mut c_void,
            creation_disposition: Dword,
            flags_and_attributes: Dword,
            template_file: Handle,
        ) -> Handle;

        pub fn ReadFile(
            file: Handle,
            buffer: *mut c_void,
            bytes_to_read: Dword,
            bytes_read: *mut Dword,
            overlapped: *mut c_void,
        ) -> Bool;

        pub fn CloseHandle(object: Handle) -> Bool;
    }
}

#[cfg(windows)]
pub fn read_via_read_file(tester: &mut RepetitionTester, dest: &mut [u8], file_name: &str) {
    use std::ptr;

    while tester.is_testing() {
        let name = match CString::new(file_name) {
            Ok(name) => name,
            Err(_) => {
                tester.error("invalid file name");
                break;
            }
        };

        let file = unsafe {
            win32::CreateFileA(
                name.as_ptr(),
                win32::GENERIC_READ,
                win32::FILE_SHARE_READ | win32::FILE_SHARE_WRITE,
                ptr::null_mut(),
                win32::OPEN_EXISTING,
                win32::FILE_ATTRIBUTE_NORMAL,
                ptr::null_mut(),
            )
        };

        if file != win32::invalid_handle_value() {
            let mut offset = 0usize;
            let mut size_remaining = dest.len() as u64;

            while size_remaining != 0 {
                let mut read_size = u32::MAX;
                if read_size as u64 > size_remaining {
                    read_size = size_remaining as u32;
                }

                let ptr = unsafe { dest.as_mut_ptr().add(offset) } as *mut c_void;
                let mut bytes_read: win32::Dword = 0;

                tester.begin_time();
                let result =
                    unsafe { win32::ReadFile(file, ptr, read_size, &mut bytes_read, ptr::null_mut()) };
                tester.end_time();

                if result != 0 && bytes_read == read_size {
                    tester.count_bytes(read_size as u64);
                } else {
                    tester.error("ReadFile failed");
                }

                size_remaining -= read_size as u64;
                offset += read_size as usize;
            }

            unsafe {
                win32::CloseHandle(file);
            }
        } else {
            tester.error("CreateFileA failed");
        }
    }
}
