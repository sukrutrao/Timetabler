/** @file */

#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include "fields/field.h"
#include "global.h"

/**
 * @brief      Enum Class for course type.
 */
enum class CourseType {
    /**
     * Value if the Course is a core course
     */
    core,
    /**
     * Value if the Course is an elective course
     */
    elective
};


/**
 * @brief      Class for a program.
 */
class Program : public Field {
private:
    /**
     * The name of the Program, which uniquely identifies it.
     * For example, it might be "B Tech 1".
     */
    std::string name; 
    /**
     * The type with which the Program has a certain Course.
     * This could be core or elective, which specifies whether the 
     * Program has a particular Course as a core course or as an elective.
     */
    CourseType courseType;
public:
    Program(std::string, CourseType);
    bool operator==(const Program &other);
    FieldType getType();
    std::string getName();
    bool isCoreProgram();
    std::string getTypeName();
    std::string getCourseTypeName();
    std::string getNameWithType();
};

#endif
