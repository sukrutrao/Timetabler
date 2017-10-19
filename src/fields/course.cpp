#include "fields/course.h"

#include <string>
#include <vector>
#include "fields/instructor.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/is_minor.h"

Course::Course(std::string name, unsigned classSize, Instructor* instructor, Segment* segment, 
            IsMinor *isMinor) {
    this->name = name;
    this->classSize = classSize;
    this->instructor = instructor;
    this->segment = segment;
    this->isMinor = isMinor;
}

Course::Course(std::string name, unsigned classSize, Instructor* instructor, Segment* segment, 
            IsMinor *isMinor, std::vector<Program*> programs) {
    this->name = name;
    this->classSize = classSize;
    this->instructor = instructor;
    this->segment = segment;
    this->isMinor = isMinor;
    this->programs = programs;
}

void Course::setPrograms(std::vector<Program*> programs) {
    this->programs = programs;
}

void Course::addProgram(Program* programs) {
    this->programs.push_back(programs);
}

bool Course::operator==(const Course &other) {
    return (this->name == other.name);
}

std::string Course::getName() {
    return name;
}


