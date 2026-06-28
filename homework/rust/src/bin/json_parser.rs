// Port of json_parser.cpp

use haversine::haversine_parser::{
    get_pair_count, read_file, sum_haversine_distances, JsonParser, JsonTokenizer,
};
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == 2 || args.len() == 3 {
        let file_content = read_file(&args[1]);
        let tokenizer = JsonTokenizer::new(&file_content);
        let mut parser = JsonParser::new(tokenizer);
        let data = parser.parse();

        let pair_count = get_pair_count(&data);
        println!("Input size: {}", file_content.len());
        println!("Pair count: {}", pair_count);
        let sum = sum_haversine_distances(pair_count, &data);
        println!("Haversine sum: {:.15}", sum);

        if args.len() == 3 {
            let bytes = std::fs::read(&args[2]).expect("failed to read answers file");
            let ref_count = bytes.len() / std::mem::size_of::<f64>() - 1;
            if pair_count as usize != ref_count {
                panic!("pairCount != refCount");
            }

            let offset = ref_count * std::mem::size_of::<f64>();
            let ref_sum = f64::from_le_bytes(bytes[offset..offset + 8].try_into().unwrap());

            println!("Reference sum: {:.15}", ref_sum);
            println!("Difference: {:.15}", sum - ref_sum);
        }
    } else {
        eprintln!("Usage: {} [haversine_input.json]", args[0]);
        eprintln!("       {} [haversine_input.json] [answers.f64]", args[0]);
        std::process::exit(1);
    }
}
