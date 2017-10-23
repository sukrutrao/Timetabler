#include "fields/classroom.h"

#include <string>
#include "global.h"

/**
 * @brief      Constructs the object.
 *
 * @param[in]  number  The Classroom number, a unique identifier
 * @param[in]  size    The number of students the Classroom can hold
 */
Classroom::Classroom(std::string number, unsigned size) {
    this->number = number;
    this->size = size;
}

/**
 * @brief      Overloads the == operator to check for equality.
 * 
 * Two classrooms are said to be equal if their numbers are identical.
 *
 * @param[in]  other  The Classroom with which the comparison is being done
 *
 * @return     Returns true if equal, false otherwise
 */
bool Classroom::operator==(const Classroom &other) {
    return (this->number == other.number);
}

/**
 * @brief      Checks if the size of this Classroom is less than the size of another.
 *
 * @param[in]  other  The Classroom with which the comparison is being done
 *
 * @return     Returns true if less, false othewise
 */
bool Classroom::sizeLessThan(const Classroom &other) {
    return (this->size < other.size);
}

/**
 * @brief      Gets the type under the FieldType enum.
 *
 * @return     A member of the FieldType enum, which is FieldType::classroom
 */
FieldType Classroom::getType() {
    return FieldType::classroom;
}

/**
 * @brief      Gets the type name, which is "Classroom".
 *
 * @return     The string "Classroom"
 */
std::string Classroom::getTypeName() {
    return "Classroom";
}

/**
 * @brief      Gets the class number of the Classroom.
 *
 * @return     The class number, which is unique identifier of the Classroom
 */
std::string Classroom::getName() {
    return number;
}

/**
 * @brief      Gets the size of the Classroom.
 *
 * @return     The size of the Classroom
 */
unsigned Classroom::getSize() {
    return size;
}
