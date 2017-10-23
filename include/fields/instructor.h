#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <string>
#include "fields/field.h"
#include "global.h"

/**
 * @brief      Class for an instructor.
 */
class Instructor : public Field {
private:
	/**
	 * The name of the instructor, that uniquely identifies the instructor.
	 */
    std::string name;
public:
    Instructor(std::string);
    bool operator==(const Instructor &other);
    FieldType getType();
    std::string getName();
    std::string getTypeName();
};

#endif
