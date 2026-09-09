#include "csv_table.hpp"

#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <utility>

csv_table::csv_table(std::string row_header,
                     std::vector<std::string> column_headers)
    : m_row_header{std::move(row_header)},
      m_column_headers{std::move(column_headers)} {}

void csv_table::add_row(std::string row_header, std::span<f64 const> values) {
  if (values.size() != m_column_headers.size()) {
    throw std::invalid_argument{"CSV row has the wrong number of values"};
  }

  m_rows.emplace_back(std::move(row_header),
                      std::vector<f64>{values.begin(), values.end()});
}

void csv_table::write_to_file(std::filesystem::path const& path) const {
  std::ofstream ofs{path};
  if (!ofs) {
    throw std::runtime_error{"Failed to open CSV file"};
  }

  ofs << m_row_header;
  for (auto const& column_header : m_column_headers) {
    ofs << ',' << column_header;
  }
  ofs << '\n';

  ofs << std::fixed << std::setprecision(6);
  for (auto const& [row_header, values] : m_rows) {
    ofs << row_header;
    for (auto const value : values) {
      ofs << ',' << value;
    }
    ofs << '\n';
  }
}
