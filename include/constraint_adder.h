/** @file */

#ifndef CONSTRAINT_ADDER_H
#define CONSTRAINT_ADDER_H

#include "clauses.h"
#include "constraint_encoder.h"
#include "core/SolverTypes.h"
#include "global.h"
class TimeTabler;

using namespace Glucose;

/**
 * @brief      Class for constraint adder.
 *
 * This class is responsible for interpreting each constraint at
 * a high level and creating clauses to represent them. This class contains
 * functions each which represents a high level constraint. In the function,
 * courses are iterated over and calls are made to an object of
 * ConstraintEncoder to get Clauses corresponding to lower level constraints for
 * a given course, which are then joined together using the defined operations.
 * It also contains functions to add these predefined constraints with their
 * prescribed weights to the TimeTabler, which then adds it to the solver.
 */
class ConstraintAdder {
  private:
    /**
     * A pointer to the ConstraintEncoder object used for getting constraints
     * for a given course
     */
    ConstraintEncoder *encoder;
    /**
     * A pointer to a TimeTabler object to access data, variables, and add
     * constraints to the solver
     */
    TimeTabler *timeTabler;
    Clauses fieldSingleValueAtATime(FieldType);
    Clauses exactlyOneFieldValuePerCourse(FieldType);
    Clauses instructorSingleCourseAtATime();
    Clauses classroomSingleCourseAtATime();
    Clauses programSingleCoreCourseAtATime();
    Clauses minorInMinorTime();
    Clauses coreInMorningTime();
    Clauses electiveInNonMorningTime();
    Clauses existingAssignmentClauses();
    Clauses programAtMostOneOfCoreOrElective();

  public:
    ConstraintAdder(ConstraintEncoder *, TimeTabler *);
    void addConstraints();
};

#endif
