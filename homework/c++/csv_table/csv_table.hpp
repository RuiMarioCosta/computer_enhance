#pragma once

#include "types.hpp"

#include <filesystem>
#include <span>
#include <string>
#include <utility>
#include <vector>

class csv_table {
public:
  csv_table(std::string row_header, std::vector<std::string> column_headers);

  void add_row(std::string row_header, std::span<f64 const> values);
  void write_to_file(std::filesystem::path const& path) const;

private:
  using row = std::pair<std::string, std::vector<f64>>;

  std::string m_row_header;
  std::vector<std::string> m_column_headers;
  std::vector<row> m_rows;
};
