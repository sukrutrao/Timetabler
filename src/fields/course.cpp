#include "fields/course.h"

#include "fields/instructor.h"
#include "fields/is_minor.h"
#include "fields/program.h"
#include "fields/segment.h"
#include <string>
#include <vector>

/**
 * @brief      Constructs the Course object.
 *
 * @param[in]  name        The name of the course
 * @param[in]  classSize   The class size of the course
 * @param[in]  instructor  The instructor index of the course
 * @param[in]  segment     The segment index of the course
 * @param[in]  isMinor     Indicates if the course is a minor course
 */
Course::Course(std::string name, unsigned classSize, int instructor,
               int segment, int isMinor) {
    this->name = name;
    this->classSize = classSize;
    this->instructor = instructor;
    this->segment = segment;
    this->isMinor = isMinor;
}

/**
 * @brief      Constructs the Course object.
 *
 * @param[in]  name        The name of the course
 * @param[in]  classSize   The class size of the course
 * @param[in]  instructor  The instructor index of the course
 * @param[in]  segment     The segment index of the course
 * @param[in]  isMinor     Indicates if the course is a minor course
 * @param[in]  programs    The indices of the programs for which the course is
 * applicable
 */
Course::Course(std::string name, unsigned classSize, int instructor,
               int segment, int isMinor, std::vector<int> programs) {
    this->name = name;
    this->classSize = classSize;
    this->instructor = instructor;
    this->segment = segment;
    this->isMinor = isMinor;
    this->programs = programs;
}

/**
 * @brief      Sets the programs for which the Course is applicable.
 *
 * @param[in]  programs  The indices of the programs for which the Course is
 * applicable
 */
void Course::setPrograms(std::vector<int> programs) {
    this->programs = programs;
}

/**
 * @brief      Adds a program that is applicable to the Course.
 *
 * @param[in]  programs  The index of the program to be added
 */
void Course::addProgram(int programs) { this->programs.push_back(programs); }

void Course::addClassroom(int cr) {
    this->classroom = cr;
}

void Course::addSlot(int s) {
    this->slot = s;
}

/**
 * @brief      Checks if two Course objects are identical, i.e.,
 *             if they represent the same Course.
 *
 * @param[in]  other  The Course with which the comparison is being made
 *
 * @return     true if identical, false otherwise
 */
bool Course::operator==(const Course &other) {
    return (this->name == other.name);
}

/**
 * @brief      Gets the name of the Course.
 *
 * @return     The name.of the Course
 */
std::string Course::getName() { return name; }

/**
 * @brief      Gets the instructor index of the Course.
 *
 * @return     The instructor index in the list of instructors
 */
int Course::getInstructor() { return instructor; }

/**
 * @brief      Gets the indices of the programs of the Course.
 *
 * @return     The indices of the programs in the list of programs
 */
std::vector<int> Course::getPrograms() { return programs; }

/**
 * @brief      Gets the segment index of the Course.
 *
 * @return     The segment index in the list of segments
 */
int Course::getSegment() { return segment; }

/**
 * @brief      Gets the 'is minor' index of the Course.
 *
 * @return     The isMinor index in the list of isMinors
 */
int Course::getIsMinor() { return isMinor; }

int Course::getClassroom() {
    return classroom;
}

int Course::getSlot() {
    return slot;
}

/**
 * @brief      Gets the class size of the Course.
 *
 * @return     The class size.
 */
unsigned Course::getClassSize() { return classSize; }