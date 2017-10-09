#include "fields/instructor.h"

#include <string>
#include "global.h"

Instructor::Instructor(std::string name) {
	this->name = name;
}

bool Instructor::operator==(const Instructor &other) {
	return (this->name == other.name);
}

FieldType Instructor::getType() {
	return FieldType::instructor;
}