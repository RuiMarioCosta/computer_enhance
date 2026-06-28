// Port of haversine_parser.hpp / haversine_parser.cpp
//
// A hand-written JSON tokenizer + recursive-descent parser, plus the helpers
// that read a file, count the coordinate pairs, and sum the haversine
// distances. Profiling macros mirror the TimeFunction / TimeBandwidth /
// TimeBlock usage in the C++ version.

use crate::haversine_math::reference_haversine;
use std::collections::HashMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum JsonTokenType {
    OpenBrace,
    OpenBracket,
    CloseBrace,
    CloseBracket,
    Comma,
    Colon,
    SemiColon,
    StringLiteral,
    Number,
    True,
    False,
    Null,
    End,
}

#[derive(Debug, Clone, Copy)]
pub struct JsonToken<'a> {
    pub token_type: JsonTokenType,
    pub value: &'a str,
}

#[derive(Debug, Clone)]
pub enum JsonValue {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<JsonValue>),
    Object(HashMap<String, JsonValue>),
}

pub struct JsonTokenizer<'a> {
    src: &'a str,
    pos: usize,
}

impl<'a> JsonTokenizer<'a> {
    pub fn new(src: &'a str) -> Self {
        JsonTokenizer { src, pos: 0 }
    }

    #[inline]
    fn bytes(&self) -> &'a [u8] {
        self.src.as_bytes()
    }

    pub fn next(&mut self) -> JsonToken<'a> {
        self.skip_ws();

        let b = self.bytes();
        if self.pos >= b.len() {
            return JsonToken {
                token_type: JsonTokenType::End,
                value: "",
            };
        }

        let c = b[self.pos];
        match c {
            b'{' => self.simple(JsonTokenType::OpenBrace),
            b'[' => self.simple(JsonTokenType::OpenBracket),
            b'}' => self.simple(JsonTokenType::CloseBrace),
            b']' => self.simple(JsonTokenType::CloseBracket),
            b':' => self.simple(JsonTokenType::Colon),
            b',' => self.simple(JsonTokenType::Comma),
            b'"' => self.string(),
            b'\0' => self.simple(JsonTokenType::End),
            _ => {
                if c.is_ascii_digit() || c == b'-' {
                    self.number()
                } else if self.starts_with("true") {
                    let start = self.pos;
                    self.pos += 4;
                    JsonToken {
                        token_type: JsonTokenType::True,
                        value: &self.src[start..self.pos],
                    }
                } else if self.starts_with("false") {
                    let start = self.pos;
                    self.pos += 5;
                    JsonToken {
                        token_type: JsonTokenType::False,
                        value: &self.src[start..self.pos],
                    }
                } else if self.starts_with("null") {
                    let start = self.pos;
                    self.pos += 4;
                    JsonToken {
                        token_type: JsonTokenType::Null,
                        value: &self.src[start..self.pos],
                    }
                } else {
                    panic!("Unexpected token type");
                }
            }
        }
    }

    fn skip_ws(&mut self) {
        let b = self.bytes();
        while self.pos < b.len() && b[self.pos].is_ascii_whitespace() {
            self.pos += 1;
        }
    }

    fn simple(&mut self, token_type: JsonTokenType) -> JsonToken<'a> {
        let start = self.pos;
        self.pos += 1;
        JsonToken {
            token_type,
            value: &self.src[start..start + 1],
        }
    }

    fn string(&mut self) -> JsonToken<'a> {
        self.pos += 1; // skip opening "
        let start = self.pos;
        let b = self.bytes();
        while self.pos < b.len() && b[self.pos] != b'"' {
            self.pos += 1;
        }
        let end = self.pos;
        self.pos += 1; // skip closing "
        JsonToken {
            token_type: JsonTokenType::StringLiteral,
            value: &self.src[start..end],
        }
    }

    fn number(&mut self) -> JsonToken<'a> {
        let start = self.pos;
        let b = self.bytes();

        if b[self.pos] == b'-' {
            self.pos += 1;
        }
        while self.pos < b.len() && b[self.pos].is_ascii_digit() {
            self.pos += 1;
        }
        if self.pos < b.len() && b[self.pos] == b'.' {
            self.pos += 1;
        }
        while self.pos < b.len() && b[self.pos].is_ascii_digit() {
            self.pos += 1;
        }

        JsonToken {
            token_type: JsonTokenType::Number,
            value: &self.src[start..self.pos],
        }
    }

    fn starts_with(&self, s: &str) -> bool {
        self.src[self.pos..].starts_with(s)
    }
}

pub struct JsonParser<'a> {
    tokenizer: JsonTokenizer<'a>,
    token: JsonToken<'a>,
}

impl<'a> JsonParser<'a> {
    pub fn new(mut tokenizer: JsonTokenizer<'a>) -> Self {
        let token = tokenizer.next();
        JsonParser { tokenizer, token }
    }

    pub fn parse(&mut self) -> JsonValue {
        crate::time_function!();

        let value = self.value();
        self.token = self.tokenizer.next();
        if self.token.token_type != JsonTokenType::End {
            panic!("Expected end of file");
        }
        value
    }

    fn value(&mut self) -> JsonValue {
        crate::time_function!();

        match self.token.token_type {
            JsonTokenType::OpenBrace => self.object(),
            JsonTokenType::OpenBracket => self.array(),
            JsonTokenType::StringLiteral => JsonValue::String(self.token.value.to_string()),
            JsonTokenType::Number => self.number(),
            JsonTokenType::True => JsonValue::Bool(true),
            JsonTokenType::False => JsonValue::Bool(false),
            JsonTokenType::Null => JsonValue::Null,
            _ => panic!("Unexpected token type"),
        }
    }

    fn object(&mut self) -> JsonValue {
        let mut object = HashMap::new();

        self.token = self.tokenizer.next();
        while self.token.token_type != JsonTokenType::End {
            let key = self.token.value.to_string();

            if self.tokenizer.next().token_type != JsonTokenType::Colon {
                panic!("Expected colon");
            }

            self.token = self.tokenizer.next();
            let value = self.value();
            object.insert(key, value);

            self.token = self.tokenizer.next();
            if self.token.token_type == JsonTokenType::CloseBrace {
                return JsonValue::Object(object);
            } else if self.token.token_type != JsonTokenType::Comma {
                panic!("Expected comma");
            }

            self.token = self.tokenizer.next();
        }

        panic!("Incorrect json object");
    }

    fn array(&mut self) -> JsonValue {
        let mut array = Vec::new();

        self.token = self.tokenizer.next();
        while self.token.token_type != JsonTokenType::End {
            let value = self.value();
            array.push(value);

            self.token = self.tokenizer.next();
            if self.token.token_type == JsonTokenType::CloseBracket {
                return JsonValue::Array(array);
            } else if self.token.token_type != JsonTokenType::Comma {
                panic!("Expected comma");
            }

            self.token = self.tokenizer.next();
        }

        panic!("Incorrect json array");
    }

    fn number(&mut self) -> JsonValue {
        let value = self.token.value.parse::<f64>().unwrap_or(0.0);
        JsonValue::Number(value)
    }
}

pub fn read_file(path: &str) -> String {
    let size = std::fs::metadata(path).map(|m| m.len()).unwrap_or(0);
    crate::time_bandwidth!("read_file", size);

    std::fs::read_to_string(path).expect("failed to read file")
}

pub fn get_pair_count(data: &JsonValue) -> u64 {
    crate::time_function!();

    if let JsonValue::Object(object) = data {
        if let Some(JsonValue::Array(pairs)) = object.get("pairs") {
            return pairs.len() as u64;
        }
    }
    panic!("Expected object with array \"pairs\"");
}

fn get_number(map: &HashMap<String, JsonValue>, key: &str) -> f64 {
    match map.get(key) {
        Some(JsonValue::Number(n)) => *n,
        _ => panic!("Missing or non-numeric key: {key}"),
    }
}

pub fn sum_haversine_distances(pair_count: u64, data: &JsonValue) -> f64 {
    crate::time_bandwidth!(
        "sum_haversine_distances",
        pair_count * std::mem::size_of::<JsonValue>() as u64
    );

    let object = match data {
        JsonValue::Object(o) => o,
        _ => panic!("Expected object"),
    };
    let pairs = match object.get("pairs") {
        Some(JsonValue::Array(a)) => a,
        _ => panic!("Expected array \"pairs\""),
    };

    let earth_radius = 6372.8;
    let mut sum = 0.0;
    {
        crate::time_block!("Lookup and Convert");
        for pair in pairs {
            if let JsonValue::Object(values) = pair {
                let x0 = get_number(values, "x0");
                let y0 = get_number(values, "y0");
                let x1 = get_number(values, "x1");
                let y1 = get_number(values, "y1");
                sum += reference_haversine(x0, y0, x1, y1, earth_radius);
            }
        }
    }
    sum / pair_count as f64
}
