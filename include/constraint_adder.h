#ifndef CONSTRAINT_ADDER_H
#define CONSTRAINT_ADDER_H

#include 

class ConstraintAdder {
private:
    Encoder encoder;
    TimeTabler *timeTabler;
    Clauses instructorSingleCourseAtATime();
    Clauses classroomSingleCourseAtATime();
    Clauses programSingleCoreCourseAtATime();
    Clauses minorInMinorTime();
//    Clauses noCoreInMinorTime();
    Clauses exactlyOneTimePerCourse();
    Clauses exactlyOneClassroomPerCourse();
    Clauses softCoreInMorningTime();
    Clauses softElectiveInAfternoonTime();
    Clauses addCustomConstraint(ClauseType, unsigned);
public:
    ConstraintAdder(Encoder, TimeTabler*);
    Clauses addConstraints();
};

#endif