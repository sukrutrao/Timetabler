#include "fields/instructor.h"

#include <string>
#include "global.h"

/**
 * @brief      Constructs the Instructor object.
 *
 * @param[in]  name  The name of the Instructor, which uniquely identifies the
 * Instructor
 */
Instructor::Instructor(std::string name) { this->name = name; }

/**
 * @brief      Checks if two Instructor objects are identical, i.e., if both
 *             have the same name.
 *
 * @param[in]  other  The Instructor with which the comparison is being made
 *
 * @return     true if identical, false otherwise
 */
bool Instructor::operator==(const Instructor &other) {
  return (this->name == other.name);
}

/**
 * @brief      Gets the type under the FieldType enum.
 *
 * @return     A member of the FieldType enum, which is FieldType::instructor
 */
FieldType Instructor::getType() { return FieldType::instructor; }

/**
 * @brief      Gets the name of the Instructor.
 *
 * @return     The name of the Instructor
 */
std::string Instructor::getName() { return name; }

/**
 * @brief      Gets the type name, which is "Instructor".
 *
 * @return     The string "Instructor"
 */
std::string Instructor::getTypeName() { return "Instructor"; }