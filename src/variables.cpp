#include "variables.h"

Variable::Variable(Course *course, Field *field) {
    this->course = course;
    this->field = field;
}

bool Variable::hasSameCourse(const Variable &other) {
    return (*(this->course) == *(other.course));
}

bool Variable::hasSameType(const Variable &other) {
    return (this->field->getType() == other.field->getType());
}

FieldType getType() {
    return field->getType();
}
