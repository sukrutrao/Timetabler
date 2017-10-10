#ifndef CONSTRAINT_ADDER_H
#define CONSTRAINT_ADDER_H

#include 

class ConstraintAdder {
private:
    Encoder encoder;
    Solver solver;
    Clauses instructorSingleCourseAtATime();
    Clauses classroomSingleCourseAtATime();
    Clauses programSingleCoreCourseAtATime();
    Clauses minorInMinorTime();
    Clauses noCoreInMinorTime();
    Clauses exactlyOneTimePerCourse();
    Clauses exactlyOneClassroomPerCourse();
    Clauses softCoreInMorningTime();
    Clauses softElectiveInAfternoonTime();
    Clauses addCustomConstraint(ClauseType, unsigned);
public:
    ConstraintAdder(Encoder, Solver);
    void addConstraints();
    bool solve();
    std::vector<Var> getResults();
};

#endif