#ifndef GLOBAL_H
#define GLOBAL_H

class TimeTabler;

enum FieldType {
    instructor = 0,
    segment = 1,
    isMinor = 2,
    program = 3,
    classroom = 4,
    slot = 5
};

enum class ClauseType {
    hard,
    soft
};

enum PredefinedClauses {
    instructorSingleCourseAtATime,
    classroomSingleCourseAtATime,
    programSingleCoreCourseAtATime,
    minorInMinorTime,
    exactlyOneSlotPerCourse,
    exactlyOneInstructorPerCourse,
    exactlyOneSegmentPerCourse,
    exactlyOneIsMinorPerCourse,
    exactlyOneClassroomPerCourse,
    coreInMorningTime
};

class Global {
public:
    static const int FIELD_COUNT = 6;
    static TimeTabler *timeTabler;
};

#endif

