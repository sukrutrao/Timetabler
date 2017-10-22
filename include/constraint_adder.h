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
    Clauses coreInMorningTime();
    Clauses existingAssignmentClauses();
    Clauses programAtMostOneOfCoreOrElective();
public:
    /*Clauses classroomSingleCourseAtATime();*/
    ConstraintAdder(ConstraintEncoder*, TimeTabler*);
    void addConstraints();
    Clauses softConstraints();
    Clauses customConstraint(FieldType, std::vector<int>, FieldType, std::vector<int>, bool);
    Clauses customConstraint(std::vector<int>, FieldType, std::vector<int>, bool);

};

#endif