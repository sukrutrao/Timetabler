#include "fields/program.h"

#include "global.h"
#include <string>

/**
 * @brief      Constructs the Program object.
 *
 * @param[in]  name        The name of the Program, which uniquely identifies
 * it. For example, it could be "B Tech 1".
 * @param[in]  courseType  The course type, which could be core or elective
 */
Program::Program(std::string name, CourseType courseType) {
    this->name = name;
    this->courseType = courseType;
}

/**
 * @brief      Checks if two Program objects are identical, which is whether
 * they have the same name and course type.
 *
 * @param[in]  other  The Program object with which the comparison is being made
 *
 * @return     True if identical, False otherwise
 */
bool Program::operator==(const Program &other) {
    return ((this->name == other.name) &&
            (this->courseType == other.courseType));
}

/**
 * @brief      Gets the type under the FieldType enum.
 *
 * @return     A member of the FieldType enum, which is FieldType::program
 */
FieldType Program::getType() { return FieldType::program; }

/**
 * @brief      Gets the name of the Program.
 *
 * @return     The name of the Program
 */
std::string Program::getName() { return name; }

/**
 * @brief      Determines if the Program has a Course as core.
 *
 * @return     True if core program, False otherwise.
 */
bool Program::isCoreProgram() { return courseType == CourseType::core; }

/**
 * @brief      Gets the type name, which is "Program".
 *
 * @return     The string "Program"
 */
std::string Program::getTypeName() { return "Program"; }

/**
 * @brief      Gets the course type name as a string.
 *
 * @return     The string "Core", if the type is core, and "Elective"
 *             if the type is elective
 */
std::string Program::getCourseTypeName() {
    if (courseType == CourseType::core) {
        return "Core";
    }
    return "Elective";
}

/**
 * @brief      Gets the name with type. The result is the program name,
 *             followed by a space, and followed by its type.
 *
 * @return     The name with type
 */
std::string Program::getNameWithType() {
    return name + " " + getCourseTypeName();
}