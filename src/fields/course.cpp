#include "fields/course.h"

#include <string>
#include <vector>
#include "fields/instructor.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/is_minor.h"

Course::Course(std::string name, unsigned classSize, int instructor, int segment, 
            int isMinor) {
    this->name = name;
    this->classSize = classSize;
    this->instructor = instructor;
    this->segment = segment;
    this->isMinor = isMinor;
}

Course::Course(std::string name, unsigned classSize, int instructor, int segment, 
            int isMinor, std::vector<int> programs) {
    this->name = name;
    this->classSize = classSize;
    this->instructor = instructor;
    this->segment = segment;
    this->isMinor = isMinor;
    this->programs = programs;
}

void Course::setPrograms(std::vector<int> programs) {
    this->programs = programs;
}

void Course::addProgram(int programs) {
    this->programs.push_back(programs);
}

bool Course::operator==(const Course &other) {
    return (this->name == other.name);
}

std::string Course::getName() {
    return name;
}

int Course::getInstructor() {
    return instructor;
}

std::vector<int> Course::getPrograms() {
    return programs;
}

int Course::getSegment() {
    return segment;
}

int Course::getIsMinor() {
    return isMinor;
}