// Port of listing_0104_read_overhead_main.cpp
//
// Repeatedly times three different file-reading APIs against a file given on
// the command line, printing the min / max / avg read time (and bandwidth) for
// each. Like the C++ original, it loops forever, refining the minimum times.

use haversine::metrics::get_cpu_freq;
use haversine::read_overhead_test::{read_via_fread, read_via_read, ReadOverheadTestFunc};
use haversine::repetition_tester::RepetitionTester;
use std::env;
use std::fs;

struct TestFunction {
    name: &'static str,
    func: ReadOverheadTestFunc,
}

fn test_functions() -> Vec<TestFunction> {
    let mut functions = vec![
        TestFunction {
            name: "fread",
            func: read_via_fread,
        },
        TestFunction {
            name: "_read",
            func: read_via_read,
        },
    ];

    #[cfg(windows)]
    {
        use haversine::read_overhead_test::read_via_read_file;
        functions.push(TestFunction {
            name: "ReadFile",
            func: read_via_read_file,
        });
    }

    functions
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let cpu_timer_freq = get_cpu_freq();

    if args.len() != 2 {
        eprintln!("Usage: {} [existing filename]", args[0]);
        return;
    }

    let file_name = &args[1];

    let file_size = match fs::metadata(file_name) {
        Ok(meta) => meta.len(),
        Err(err) => {
            eprintln!("ERROR: unable to stat \"{}\": {}", file_name, err);
            return;
        }
    };

    if file_size == 0 {
        eprintln!("ERROR: Test data size must be non-zero");
        return;
    }

    let mut dest = vec![0u8; file_size as usize];

    let functions = test_functions();
    let mut testers: Vec<RepetitionTester> =
        (0..functions.len()).map(|_| RepetitionTester::new()).collect();

    loop {
        for (func_index, test_func) in functions.iter().enumerate() {
            let tester = &mut testers[func_index];

            println!("\n--- {} ---", test_func.name);
            tester.new_test_wave(dest.len() as u64, cpu_timer_freq, 10);
            (test_func.func)(tester, &mut dest, file_name);
        }
    }
}
