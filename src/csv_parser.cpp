#include "csv_parser.h"

#include <tao/pegtl.hpp>
#include "utils.h"

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace csv_grammar {

template <typename Rule>
struct action : pegtl::nothing<Rule> {};

struct string_without_quote : pegtl::star<pegtl::not_one<'"', 10, 13>> {};
template <>
struct action<string_without_quote> {
  template <typename Input>
  static void apply(const Input &in, CSVParser &csv) {
    csv.temp = in.string();
  }
};

struct plain_value : pegtl::star<pegtl::not_one<',', 10, 13>> {};
template <>
struct action<plain_value> {
  template <typename Input>
  static void apply(const Input &in, CSVParser &csv) {
    csv.temp = in.string();
  }
};

struct quoted_vale
    : pegtl::if_must<pegtl::one<'"'>, string_without_quote, pegtl::one<'"'>> {};

struct value : pegtl::sor<quoted_vale, plain_value> {};
template <>
struct action<value> {
  template <typename Input>
  static void apply(const Input &in, CSVParser &csv) {
    csv.current.push_back(csv.temp);
    csv.temp = "";
  }
};

struct line : pegtl::seq<pegtl::list<value, pegtl::one<','>>, pegtl::eol> {};
template <>
struct action<line> {
  template <typename Input>
  static void apply(const Input &in, CSVParser &csv) {
    if (csv.parsing_header) {
      csv.header = csv.current;
      csv.current.clear();
      csv.parsing_header = false;
    } else if (csv.current.size() > 0) {
      if (csv.current.size() == 1) {
        if (csv.current[0] == "") return;
      }
      if (csv.current.size() != csv.header.size()) {
        LOG(ERROR) << in.position() << " Number of columns ("
                   << csv.current.size() << ") different from the header ("
                   << csv.header.size() << ")";
      }
      csv.data.push_back(CSVParser::Row(csv.header, csv.current));
      csv.current.clear();
    }
  }
};

struct file : pegtl::until<pegtl::eof, line> {};
template <>
struct action<file> {
  template <typename Input>
  static void apply(const Input &in, CSVParser &csv) {}
};

template <typename Rule>
struct control : pegtl::normal<Rule> {
  template <typename Input, typename... States>
  static void raise(const Input &in, States &&...) {
    LOG(ERROR) << in.position() << " Error parsing CSV file";
  }
};

};  // namespace csv_grammar

CSVParser::CSVParser(std::string file) {
  pegtl::file_input<> in(file);
  pegtl::parse<pegtl::must<csv_grammar::file>, csv_grammar::action,
               csv_grammar::control>(in, *this);
}

unsigned CSVParser::rowCount() { return data.size(); }

std::string CSVParser::Row::operator[](const std::string i) {
  auto it = std::find(header.begin(), header.end(), i);
  if (it == header.end()) {
    LOG(ERROR) << "CSV: Invalid header name.";
  }
  unsigned index = std::distance(header.begin(), it);
  return row[index];
}

CSVParser::Row CSVParser::operator[](const unsigned i) { return data[i]; }
