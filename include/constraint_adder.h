#ifndef CONSTRAINT_ADDER_H
#define CONSTRAINT_ADDER_H

#include "encoder.h"
#include "clauses.h"
#include "global.h"
#include "core/SolverTypes.h"
class TimeTabler;

using namespace Minisat; 

class ConstraintAdder {
private:
    Encoder *encoder;
    TimeTabler *timeTabler;
    Clauses fieldSingleValueAtATime(FieldType);
    Clauses exactlyOneFieldValuePerCourse(FieldType);
    Clauses instructorSingleCourseAtATime();
    Clauses classroomSingleCourseAtATime();
    Clauses programSingleCoreCourseAtATime();
    Clauses minorInMinorTime();
//    Clauses noCoreInMinorTime();
    // Clauses exactlyOneTimePerCourse();
    // Clauses exactlyOneClassroomPerCourse();
    Clauses softCoreInMorningTime();
    Clauses softElectiveInAfternoonTime();
    Clauses addCustomConstraint(ClauseType, unsigned);
public:
    ConstraintAdder(Encoder*, TimeTabler*);
    Clauses addConstraints();
};

#endif