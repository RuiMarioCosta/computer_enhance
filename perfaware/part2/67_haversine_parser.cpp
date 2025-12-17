#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

#include "listing_0065_haversine_formula.cpp"

enum class json_token_type {
  Open_brace,
  Open_bracket,
  Close_brace,
  Close_bracket,
  Comma,
  Colon,
  Semi_colon,
  String_literal,
  Number,
  True,
  False,
  Null,

  End,

  Count,
};

struct json_token {
  json_token_type type;
  std::string_view value;
};

class JsonTokenizer {
public:
  explicit JsonTokenizer(std::string_view sv) : m_src{sv}, m_pos{0} {}

  json_token next() {
    _skip_ws();

    if (m_pos > m_src.size()) {
      return json_token{json_token_type::End, ""};
    }

    char c = m_src[m_pos];
    switch (c) {
    case '{': {
      return _simple(json_token_type::Open_brace);
    }
    case '[': {
      return _simple(json_token_type::Open_bracket);
    }
    case '}': {
      return _simple(json_token_type::Close_brace);
    }
    case ']': {
      return _simple(json_token_type::Close_bracket);
    }
    case ':': {
      return _simple(json_token_type::Colon);
    }
    case ',': {
      return _simple(json_token_type::Comma);
    }
    case '"': {
      return _string();
    }
    case '\0': {
      return _simple(json_token_type::End);
    }
    default:
      if (std::isdigit(c) || c == '-') {
        return _number();
      } else if (_starts_with("true")) {
        auto start = m_pos;
        m_pos += 4;
        return json_token{json_token_type::True,
                          m_src.substr(start, m_pos - start)};
      } else if (_starts_with("false")) {
        auto start = m_pos;
        m_pos += 5;
        return json_token{json_token_type::False,
                          m_src.substr(start, m_pos - start)};
      } else if (_starts_with("null")) {
        auto start = m_pos;
        m_pos += 4;
        return json_token{json_token_type::Null,
                          m_src.substr(start, m_pos - start)};
      }
    }

    throw std::runtime_error{"Unexpected token type"};
  }

private:
  std::string_view m_src;
  size_t m_pos;

  void _skip_ws() {
    while (m_pos < m_src.size() &&
           std::isspace(static_cast<int>(m_src[m_pos]))) {
      m_pos++;
    }
  }

  json_token _simple(json_token_type type) {
    auto start = m_pos;
    m_pos++;
    return json_token{type, m_src.substr(start, 1)};
  }

  json_token _string() {
    m_pos++;
    auto start = m_pos;
    while (m_pos < m_src.size() && m_src[m_pos] != '\"') {
      m_pos++;
    }
    if (m_pos > m_src.size()) {
      throw std::runtime_error{"Unterminated string"};
    }

    m_pos++; // skip closing "
    return json_token{json_token_type::String_literal,
                      m_src.substr(start, m_pos - 1 - start)};
  }

  json_token _number() {
    auto start = m_pos;

    if (m_src[m_pos] == '-') {
      m_pos++;
    }

    while (m_pos < m_src.size() &&
           std::isdigit(static_cast<unsigned int>(m_src[m_pos]))) {
      m_pos++;
    }

    if (m_src[m_pos] == '.') {
      m_pos++;
    }

    while (m_pos < m_src.size() &&
           std::isdigit(static_cast<unsigned int>(m_src[m_pos]))) {
      m_pos++;
    }

    return json_token{json_token_type::Number,
                      m_src.substr(start, m_pos - start)};
  }

  bool _starts_with(std::string_view sv) {
    return m_src.substr(m_pos, sv.size()) == sv;
  }
};

struct JsonValue;

using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

struct JsonValue
    : public std::variant<std::nullptr_t, bool, double, std::string_view,
                          JsonArray, JsonObject> {
  using variant::variant;
};

class JsonParser {
  JsonTokenizer m_tokenizer;
  json_token m_token;

public:
  JsonParser(JsonTokenizer tokenizer)
      : m_tokenizer{std::move(tokenizer)}, m_token{m_tokenizer.next()} {}

  JsonValue parse() {
    auto value = _value();
    m_token = m_tokenizer.next();
    if (m_token.type != json_token_type::End) {
      throw std::runtime_error{"Expected end of file"};
    }
    return value;
  }

private:
  JsonValue _value() {
    switch (m_token.type) {
    case json_token_type::Open_brace: {
      return _object();
    }
    case json_token_type::Open_bracket: {
      return _array();
    }
    case json_token_type::String_literal: {
      return std::string_view{};
    }
    case json_token_type::Number: {
      return _number();
    }
    case json_token_type::True: {
      return true;
    }
    case json_token_type::False: {
      return false;
    }
    case json_token_type::Null: {
      return nullptr;
    }
    default:
      throw std::runtime_error{"Unexpected token type"};
    }
  }

  JsonValue _object() {
    JsonObject object{};
    std::string key;
    for (m_token = m_tokenizer.next(); m_token.type != json_token_type::End;
         m_token = m_tokenizer.next()) {
      key = m_token.value;

      if (m_tokenizer.next().type != json_token_type::Colon) {
        throw std::runtime_error{"Expected colon"};
      }

      m_token = m_tokenizer.next();
      auto value = _value();
      object[key] = value;

      m_token = m_tokenizer.next();
      if (m_token.type == json_token_type::Close_brace) {
        return object;
      } else if (m_token.type != json_token_type::Comma) {
        throw std::runtime_error{"Expected comma"};
      }
    }

    throw std::runtime_error{"Incorrect json object"};
  }

  JsonValue _array() {
    JsonArray array{};
    for (m_token = m_tokenizer.next(); m_token.type != json_token_type::End;
         m_token = m_tokenizer.next()) {
      auto value = _value();
      array.push_back(value);

      m_token = m_tokenizer.next();
      if (m_token.type == json_token_type::Close_bracket) {
        return array;
      } else if (m_token.type != json_token_type::Comma) {
        throw std::runtime_error{"Expected comma"};
      }
    }

    throw std::runtime_error{"Incorrect json array"};
  }

  JsonValue _number() {
    auto value = std::strtod(m_token.value.data(), nullptr);
    return value;
  }
};

std::string read_file(std::filesystem::path const &path) {
  std::ifstream fp(path, std::ios::in | std::ios::binary);
  std::stringstream buffer;
  buffer << fp.rdbuf();
  return buffer.str();
}

u64 getPairCount(JsonValue data) {
  auto object = std::get<JsonObject>(data);
  auto pairs = std::get<JsonArray>(object["pairs"]);
  return pairs.size();
}

f64 sumHaversineDistances(u64 PairCount, JsonValue data) {
  auto object = std::get<JsonObject>(data);
  auto pairs = std::get<JsonArray>(object["pairs"]);

  f64 EarthRadius = 6372.8;
  f64 sum = 0;
  for (auto const &pair : pairs) {
    auto values = std::get<JsonObject>(pair);
    auto x0 = std::get<double>(values.at("x0"));
    auto y0 = std::get<double>(values.at("y0"));
    auto x1 = std::get<double>(values.at("x1"));
    auto y1 = std::get<double>(values.at("y1"));
    f64 dis = ReferenceHaversine(x0, y0, x1, y1, EarthRadius);
    sum += dis;
  }
  sum /= PairCount;
  return sum;
}
