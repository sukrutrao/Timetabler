#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <string>
#include "fields/field.h"
#include "global.h"

class Instructor : public Field {
private:
    std::string name;
public:
    Instructor(std::string);
    bool operator==(const Instructor &other);
    FieldType getType();
};

#endif
