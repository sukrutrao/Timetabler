#ifndef CONSTRAINT_ADDER_H
#define CONSTRAINT_ADDER_H

#include "constraint_encoder.h"
#include "clauses.h"
#include "global.h"
#include "core/SolverTypes.h"
class TimeTabler;

using namespace Minisat; 

class ConstraintAdder {
private:
    ConstraintEncoder *encoder;
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
    Clauses existingAssignmentClausesSoft();
public:
    ConstraintAdder(ConstraintEncoder*, TimeTabler*);
    Clauses addConstraints();
    Clauses softConstraints();
};

#endif