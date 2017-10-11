#ifndef PARSER_H
#define PARSER_H

#include <yaml-cpp/yaml.h>
#include "CSVParser.hpp"
#include "data.h"
#include <string>

class Parser {
	TimeTabler *timeTabler;
	Day getDayFromString(std::string &);
public:
	Parser(TimeTabler *);
    void parseFields(std::string &file);
    void parseInput(std::string &file);
};

#endif