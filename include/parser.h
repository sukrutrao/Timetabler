#ifndef PARSER_H
#define PARSER_H

#include <yaml-cpp/yaml.h>
#include "CSVParser.hpp"
#include "fields/course.h"
#include "fields/course_type.h"
#include "fields/instructor.h"
#include "fields/classroom.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/slot.h"
#include <string>

class Parser {
    std::vector<Course> courses;
    std::vector<Instructor> instructors;
    std::vector<Classroom> classrooms;
    std::vector<Program> programs;
public:
    void parseFields(std::string &file);
    void parseInput(std::string &file);
};

#endif