#ifndef PARSER_H
#define PARSER_H

#include <yaml-cpp/yaml.h>
#include "CSVparser.hpp"
#include "data.h"
#include "time_tabler.h"
#include <string>

/**
 * @brief      Class for parser.
 * 
 * This class is responsible for parsing the field and inputs
 * given by the user, and populating the corresponding data members.
 */
class Parser {
	/**
	 * A pointer to the TimeTabler object
	 */
    TimeTabler *timeTabler;
    Day getDayFromString(std::string);
public:
    Parser(TimeTabler *);
    void parseFields(std::string file);
    void parseInput(std::string file);
    void addVars();
};

#endif