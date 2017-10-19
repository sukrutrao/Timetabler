#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include "fields/instructor.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/is_minor.h"

class Course {
private:
    std::string name;
    unsigned classSize;
    int instructor;
    std::vector<int> programs;
    int segment;
    int isMinor;
public:
    Course(std::string, unsigned, int, int, int);
    Course(std::string, unsigned, int, int, int, std::vector<int>);
    void setPrograms(std::vector<int>);
    void addProgram(int);
    bool operator==(const Course &other);
    std::string getName();
    int getInstructor();
    std::vector<int> getPrograms();
    int getSegment();
    int getIsMinor();
};

#endif
