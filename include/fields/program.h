#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include "fields/field.h"
#include "global.h"

enum class CourseType {
    core,
    elective
};

class Program : public Field {
private:
    std::string name; 
    CourseType courseType;
public:
    Program(std::string, CourseType);
    bool operator==(const Program &other);
    FieldType getType();
    std::string getName();
    bool isCoreProgram();
    std::string getTypeName();
};

#endif
