#ifndef PARSER_H
#define PARSER_H

#include <yaml-cpp/yaml.h>
#include "CSVparser.hpp"
#include "data.h"
#include "time_tabler.h"
#include <string>

class Parser {
    TimeTabler *timeTabler;
    Day getDayFromString(std::string);
public:
    Parser(TimeTabler *);
    void parseFields(std::string &file);
    void parseInput(std::string &file);
    void addVars();
};

#endif