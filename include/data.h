#ifndef DATA_H
#define DATA_H

#include "fields/course.h"
#include "fields/course_type.h"
#include "fields/instructor.h"
#include "fields/classroom.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/slot.h"
#include <vector>

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