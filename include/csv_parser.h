/** @file */

#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <vector>

struct CSVParser {
  CSVParser(std::string file);
  unsigned rowCount();

  struct Row {
    Row(std::vector<std::string> &h, std::vector<std::string> r) : header(h) {
      row = r;
    }
    std::vector<std::string> row;
    std::vector<std::string> &header;
    std::string operator[](const std::string i);
  };

  Row operator[](const unsigned i);

  bool parsing_header = true;
  std::string temp;
  std::vector<std::string> current;
  std::vector<std::string> header;
  std::vector<Row> data;
};

#endif
