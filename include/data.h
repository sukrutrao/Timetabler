#ifndef DATA_H
#define DATA_H

#include "vector.h"

class Data {
public:
	std::vector<Course> courses;
    std::vector<Instructor> instructors;
    std::vector<Classroom> classrooms;
    std::vector<Program> programs;
    std::vector<Segment> segments;
    std::vector<Slot> slots;
};

#endif