#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <string>
#include "fields/field.h"
#include "global.h"

/**
 * @brief      Class for a classroom.
 */
class Classroom : public Field {
private:
	/**
	 * Represents the classroom number, which uniquely identifies the Classroom
	 */
    std::string number;
    /**
     * The size of the Classroom, which is the maximum number of students it can hold
     */
    unsigned size;
public:
    Classroom(std::string, unsigned);
    bool operator==(const Classroom &other);
    bool sizeLessThan(const Classroom &other);
    FieldType getType();
    std::string getTypeName();
    std::string getName();
    unsigned getSize();
};

#endif
