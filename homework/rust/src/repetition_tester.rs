// Port of listing_0103_repetition_tester.cpp
//
// A "repetition tester" repeatedly runs a piece of code for a fixed wall-clock
// budget, tracking the minimum / maximum / average elapsed time so that the
// fastest observed run (the one least disturbed by the OS, other processes,
// etc.) can be used as the estimate of the code's true cost.
//
// The C++ original uses unsigned wraparound arithmetic on the accumulated time
// (it subtracts the timer on BeginTime and adds it back on EndTime). We mirror
// that with wrapping_sub / wrapping_add.

use crate::metrics::read_cpu_timer;
use std::io::{self, Write};

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum TestMode {
    Uninitialized,
    Testing,
    Completed,
    Error,
}

#[derive(Clone, Copy)]
pub struct RepetitionTestResults {
    pub test_count: u64,
    pub total_time: u64,
    pub max_time: u64,
    pub min_time: u64,
}

impl Default for RepetitionTestResults {
    fn default() -> Self {
        RepetitionTestResults {
            test_count: 0,
            total_time: 0,
            max_time: 0,
            min_time: 0,
        }
    }
}

pub struct RepetitionTester {
    target_processed_byte_count: u64,
    cpu_timer_freq: u64,
    try_for_time: u64,
    tests_started_at: u64,

    mode: TestMode,
    print_new_minimums: bool,
    open_block_count: u32,
    close_block_count: u32,
    time_accumulated_on_this_test: u64,
    bytes_accumulated_on_this_test: u64,

    results: RepetitionTestResults,
}

impl Default for RepetitionTester {
    fn default() -> Self {
        RepetitionTester {
            target_processed_byte_count: 0,
            cpu_timer_freq: 0,
            try_for_time: 0,
            tests_started_at: 0,
            mode: TestMode::Uninitialized,
            print_new_minimums: false,
            open_block_count: 0,
            close_block_count: 0,
            time_accumulated_on_this_test: 0,
            bytes_accumulated_on_this_test: 0,
            results: RepetitionTestResults::default(),
        }
    }
}

fn seconds_from_cpu_time(cpu_time: f64, cpu_timer_freq: u64) -> f64 {
    if cpu_timer_freq != 0 {
        cpu_time / cpu_timer_freq as f64
    } else {
        0.0
    }
}

pub fn print_time(label: &str, cpu_time: f64, cpu_timer_freq: u64, byte_count: u64) {
    print!("{}: {:.0}", label, cpu_time);
    if cpu_timer_freq != 0 {
        let seconds = seconds_from_cpu_time(cpu_time, cpu_timer_freq);
        print!(" ({}ms)", format!("{:.6}", 1000.0 * seconds));

        if byte_count != 0 {
            let gigabyte = 1024.0 * 1024.0 * 1024.0;
            let best_bandwidth = byte_count as f64 / (gigabyte * seconds);
            print!(" {}gb/s", format!("{:.6}", best_bandwidth));
        }
    }
}

fn print_results(results: RepetitionTestResults, cpu_timer_freq: u64, byte_count: u64) {
    print_time("Min", results.min_time as f64, cpu_timer_freq, byte_count);
    println!();

    print_time("Max", results.max_time as f64, cpu_timer_freq, byte_count);
    println!();

    if results.test_count != 0 {
        print_time(
            "Avg",
            results.total_time as f64 / results.test_count as f64,
            cpu_timer_freq,
            byte_count,
        );
        println!();
    }
}

impl RepetitionTester {
    pub fn new() -> Self {
        RepetitionTester::default()
    }

    pub fn error(&mut self, message: &str) {
        self.mode = TestMode::Error;
        eprintln!("ERROR: {}", message);
    }

    pub fn new_test_wave(
        &mut self,
        target_processed_byte_count: u64,
        cpu_timer_freq: u64,
        seconds_to_try: u32,
    ) {
        match self.mode {
            TestMode::Uninitialized => {
                self.mode = TestMode::Testing;
                self.target_processed_byte_count = target_processed_byte_count;
                self.cpu_timer_freq = cpu_timer_freq;
                self.print_new_minimums = true;
                self.results.min_time = u64::MAX;
            }
            TestMode::Completed => {
                self.mode = TestMode::Testing;

                if self.target_processed_byte_count != target_processed_byte_count {
                    self.error("TargetProcessedByteCount changed");
                }

                if self.cpu_timer_freq != cpu_timer_freq {
                    self.error("CPU frequency changed");
                }
            }
            _ => {}
        }

        self.try_for_time = seconds_to_try as u64 * cpu_timer_freq;
        self.tests_started_at = read_cpu_timer();
    }

    pub fn begin_time(&mut self) {
        self.open_block_count += 1;
        self.time_accumulated_on_this_test = self
            .time_accumulated_on_this_test
            .wrapping_sub(read_cpu_timer());
    }

    pub fn end_time(&mut self) {
        self.close_block_count += 1;
        self.time_accumulated_on_this_test = self
            .time_accumulated_on_this_test
            .wrapping_add(read_cpu_timer());
    }

    pub fn count_bytes(&mut self, byte_count: u64) {
        self.bytes_accumulated_on_this_test += byte_count;
    }

    pub fn is_testing(&mut self) -> bool {
        if self.mode == TestMode::Testing {
            let current_time = read_cpu_timer();

            // NOTE(casey): We don't count tests that had no timing blocks - we
            // assume they took some other path.
            if self.open_block_count != 0 {
                if self.open_block_count != self.close_block_count {
                    self.error("Unbalanced BeginTime/EndTime");
                }

                if self.bytes_accumulated_on_this_test != self.target_processed_byte_count {
                    self.error("Processed byte count mismatch");
                }

                if self.mode == TestMode::Testing {
                    let elapsed_time = self.time_accumulated_on_this_test;
                    self.results.test_count += 1;
                    self.results.total_time += elapsed_time;
                    if self.results.max_time < elapsed_time {
                        self.results.max_time = elapsed_time;
                    }

                    if self.results.min_time > elapsed_time {
                        self.results.min_time = elapsed_time;

                        // NOTE(casey): Whenever we get a new minimum time, we
                        // reset the clock to the full trial time.
                        self.tests_started_at = current_time;

                        if self.print_new_minimums {
                            print_time(
                                "Min",
                                self.results.min_time as f64,
                                self.cpu_timer_freq,
                                self.bytes_accumulated_on_this_test,
                            );
                            print!("               \r");
                            let _ = io::stdout().flush();
                        }
                    }

                    self.open_block_count = 0;
                    self.close_block_count = 0;
                    self.time_accumulated_on_this_test = 0;
                    self.bytes_accumulated_on_this_test = 0;
                }
            }

            if (current_time - self.tests_started_at) > self.try_for_time {
                self.mode = TestMode::Completed;

                print!("                                                          \r");
                let _ = io::stdout().flush();
                print_results(
                    self.results,
                    self.cpu_timer_freq,
                    self.target_processed_byte_count,
                );
            }
        }

        self.mode == TestMode::Testing
    }
}
