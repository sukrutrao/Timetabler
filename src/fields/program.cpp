#include "fields/program.h"

#include <string>
#include "global.h"

Program::Program(std::string name, CourseType courseType) {
    this->name = name;
    this->courseType = courseType;
}

bool Program::operator==(const Program &other) {
    return (this->name == other.name);
}

FieldType Program::getType() {
    return FieldType::program;
}

std::string Program::getName() {
	return name;
}