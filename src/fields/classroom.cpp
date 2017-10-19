#include "fields/classroom.h"

#include <string>
#include "global.h"

Classroom::Classroom(std::string number, unsigned size) {
    this->number = number;
    this->size = size;
}

bool Classroom::operator==(const Classroom &other) {
    return (this->number == other.number);
}

bool Classroom::sizeLessThan(const Classroom &other) {
    return (this->size < other.size);
}

FieldType Classroom::getType() {
    return FieldType::classroom;
}

std::string Classroom::getTypeName() {
	return "Classroom";
}

std::string Classroom::getName() {
	return number;
}