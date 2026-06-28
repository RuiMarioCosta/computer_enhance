// Port of haversine_generator.cpp
//
// Generates a JSON file of coordinate pairs plus a binary ".f64" answer file
// containing each haversine distance followed by the expected average.
//
// The C++ version uses std::mt19937 + std::uniform_real_distribution. To keep
// behaviour equivalent and dependency-free, we re-implement MT19937 (the
// standard Mersenne Twister) and a uniform real distribution here. Note that
// libstdc++'s uniform_real_distribution bit layout is implementation-defined,
// so the exact numbers differ from the C++ build, but the answer file is
// self-consistent (json_parser validates against it).

use haversine::haversine_math::reference_haversine;
use std::env;
use std::fs::File;
use std::io::{BufWriter, Write};

struct Mt19937 {
    mt: [u32; 624],
    index: usize,
}

impl Mt19937 {
    fn new(seed: u32) -> Self {
        let mut mt = [0u32; 624];
        mt[0] = seed;
        for i in 1..624 {
            mt[i] = 1812433253u32
                .wrapping_mul(mt[i - 1] ^ (mt[i - 1] >> 30))
                .wrapping_add(i as u32);
        }
        Mt19937 { mt, index: 624 }
    }

    fn generate(&mut self) {
        for i in 0..624 {
            let y = (self.mt[i] & 0x8000_0000) | (self.mt[(i + 1) % 624] & 0x7fff_ffff);
            self.mt[i] = self.mt[(i + 397) % 624] ^ (y >> 1);
            if y & 1 != 0 {
                self.mt[i] ^= 0x9908_b0df;
            }
        }
        self.index = 0;
    }

    fn next_u32(&mut self) -> u32 {
        if self.index >= 624 {
            self.generate();
        }
        let mut y = self.mt[self.index];
        self.index += 1;
        y ^= y >> 11;
        y ^= (y << 7) & 0x9d2c_5680;
        y ^= (y << 15) & 0xefc6_0000;
        y ^= y >> 18;
        y
    }

    // 53-bit uniform double in [0, 1).
    fn next_f64_01(&mut self) -> f64 {
        let a = (self.next_u32() >> 5) as u64; // 27 bits
        let b = (self.next_u32() >> 6) as u64; // 26 bits
        (a as f64 * 67_108_864.0 + b as f64) / 9_007_199_254_740_992.0
    }
}

#[derive(Clone, Copy)]
struct Uniform {
    min: f64,
    max: f64,
}

impl Uniform {
    fn new(min: f64, max: f64) -> Self {
        Uniform { min, max }
    }

    fn sample(&self, rng: &mut Mt19937) -> f64 {
        self.min + (self.max - self.min) * rng.next_f64_01()
    }
}

fn get_distribution(center: f64, radius: f64, limit: f64) -> Uniform {
    let mut min = center - radius;
    if min < -limit {
        min = -limit;
    }
    let mut max = center + radius;
    if max > limit {
        max = limit;
    }
    Uniform::new(min, max)
}

fn main() {
    let earth_radius = 6372.8;
    let max_pair_count: u64 = 1u64 << 34;

    let args: Vec<String> = env::args().collect();
    if args.len() == 4 {
        let mut method_name = args[1].clone();
        let seed_value: u64 = args[2].parse().unwrap_or(0);
        let pair_count: u64 = args[3].parse().unwrap_or(0);

        if method_name != "cluster" && method_name != "uniform" {
            method_name = "uniform".to_string();
            eprintln!("WARNING: Unrecognized method name. Using 'uniform'.");
        }

        if pair_count > max_pair_count {
            eprintln!(
                "To avoid accidentally generating massive files, number of pairs must be less than {}",
                max_pair_count
            );
            std::process::exit(1);
        }

        let max_allowed_x = 180.0;
        let max_allowed_y = 90.0;

        let mut x_gen = Mt19937::new(seed_value as u32);
        let mut x_dis = Uniform::new(-max_allowed_x, max_allowed_x);
        let x_center_dis = Uniform::new(-max_allowed_x, max_allowed_x);
        let x_radius_dis = Uniform::new(0.0, max_allowed_x);

        let mut y_gen = Mt19937::new(seed_value as u32);
        let mut y_dis = Uniform::new(-max_allowed_y, max_allowed_y);
        let y_center_dis = Uniform::new(-max_allowed_y, max_allowed_y);
        let y_radius_dis = Uniform::new(0.0, max_allowed_y);

        let json_file = format!("data_{}_flex.json", pair_count);
        let mut json = BufWriter::new(File::create(&json_file).expect("create json file"));
        let haver_file = format!("data_{}_haveranswer.f64", pair_count);
        let mut haver = BufWriter::new(File::create(&haver_file).expect("create answer file"));

        write!(json, "{{\"pairs\":[\n").unwrap();

        let mut sum = 0.0;
        for n in 0..pair_count {
            if n % 64 == 0 {
                let x_center = x_center_dis.sample(&mut x_gen);
                let y_center = y_center_dis.sample(&mut y_gen);
                let x_radius = x_radius_dis.sample(&mut x_gen);
                let y_radius = y_radius_dis.sample(&mut y_gen);
                x_dis = get_distribution(x_center, x_radius, max_allowed_x);
                y_dis = get_distribution(y_center, y_radius, max_allowed_y);
            }

            let x0 = x_dis.sample(&mut x_gen);
            let x1 = x_dis.sample(&mut x_gen);
            let y0 = y_dis.sample(&mut y_gen);
            let y1 = y_dis.sample(&mut y_gen);

            let haversine_distance = reference_haversine(x0, y0, x1, y1, earth_radius);
            sum += haversine_distance;

            if n > 0 {
                write!(json, ",\n").unwrap();
            }
            write!(
                json,
                "\t{{\"x0\":{:.16}, \"y0\":{:.16}, \"x1\":{:.16}, \"y1\":{:.16}}}",
                x0, y0, x1, y1
            )
            .unwrap();
            haver.write_all(&haversine_distance.to_le_bytes()).unwrap();
        }

        write!(json, "\n]}}").unwrap();
        sum /= pair_count as f64;
        haver.write_all(&sum.to_le_bytes()).unwrap();

        println!("Method: {}", method_name);
        println!("Random seed: {}", seed_value);
        println!("Pair count: {}", pair_count);
        println!("Expected sum: {}", sum);
    } else {
        eprintln!(
            "Usage: {} [uniform/cluster] [random seed] [number of coordinate pairs to generate]",
            args[0]
        );
        std::process::exit(1);
    }
}
