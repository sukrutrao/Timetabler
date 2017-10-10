#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include "fields/field.h"
#include "global.h"

enum class CourseType : public Field {
    core,
    elective
};

class Program {
private:
    std::string name; 
    CourseType courseType;
public:
    Program(std::string, CourseType);
    bool operator==(const Program &other);
    FieldType getType();
};

#endif
