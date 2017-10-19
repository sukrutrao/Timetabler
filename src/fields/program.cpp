#include "fields/program.h"

#include <string>
#include "global.h"

Program::Program(std::string name, CourseType courseType) {
    this->name = name;
    this->courseType = courseType;
}

bool Program::operator==(const Program &other) {
    return ((this->name == other.name) && (this->courseType == other.courseType));
}

FieldType Program::getType() {
    return FieldType::program;
}

std::string Program::getName() {
	return name;
}

bool Program::isCoreProgram() {
	return courseType == CourseType::core;
}

std::string Program::getTypeName() {
	return "Programs";
}