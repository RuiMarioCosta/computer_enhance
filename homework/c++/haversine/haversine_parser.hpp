#include "types.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

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

struct JsonValue;

using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

struct JsonValue
    : public std::variant<std::nullptr_t, bool, double, std::string_view,
                          JsonArray, JsonObject> {
  using variant::variant;
};

class JsonTokenizer {
public:
  explicit JsonTokenizer(std::string_view sv);

  json_token next();

private:
  std::string_view m_src;
  size_t m_pos;

  void _skip_ws();
  json_token _simple(json_token_type type);
  json_token _string();
  json_token _number();
  bool _starts_with(std::string_view sv);
};

class JsonParser {
  JsonTokenizer m_tokenizer;
  json_token m_token;

public:
  JsonParser(JsonTokenizer tokenizer);

  JsonValue parse();

private:
  JsonValue _value();
  JsonValue _object();
  JsonValue _array();
  JsonValue _number();
};

std::string read_file(std::filesystem::path const &path);
u64 getPairCount(JsonValue data);
f64 sumHaversineDistances(u64 PairCount, JsonValue data);
