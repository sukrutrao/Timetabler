/** @file */

#ifndef CONSTRAINT_ENCODER_H
#define CONSTRAINT_ENCODER_H

#include <vector>
#include "clauses.h"
#include "core/SolverTypes.h"
#include "global.h"
#include "time_tabler.h"

using namespace NSPACE;

/**
 * @brief      Class for constraint encoder.
 *
 * This class is responsible for imposing constraints given a Course
 * or a pair of Courses and a FieldType. The constraints are of a higher
 * level than the operations of Clauses and of a lower level than
 * the constraints defined in ConstraintAdder, which are the final constraints
 * added to the solver. The class keeps a copy of the variables used
 * for every 3-tuple of (Course, FieldType, field value) and a pointer to
 * a Timetabler object to access the field data. Clauses for these constraints
 * are created using the operations defined in Clauses.
 */
class ConstraintEncoder {
 private:
  /**
   * Stores a copy of all the variables in the form of the 3-tuple
   * of (Course, FieldType, field value)
   */
  std::vector<std::vector<std::vector<Var>>> vars;
  /**
   * A pointer to a Timetabler object for accessing field data
   */
  Timetabler *timetabler;
  std::vector<Var> getAllowedVars(int, FieldType);

 public:
  ConstraintEncoder(Timetabler *);
  Clauses hasSameFieldTypeAndValue(int, int, FieldType);
  Clauses hasSameFieldTypeNotSameValue(int, int, FieldType);
  Clauses notIntersectingTime(int, int);
  Clauses notIntersectingTimeField(int, int, FieldType);
  Clauses hasExactlyOneFieldValueTrue(int, FieldType);
  Clauses hasAtLeastOneFieldValueTrue(int, FieldType);
  Clauses hasAtMostOneFieldValueTrue(int, FieldType);
  Clauses hasCommonProgram(int, int);
  Clauses hasNoCommonCoreProgram(int, int);
  Clauses isMinorCourse(int);
  Clauses slotInMinorTime(int);
  Clauses isCoreCourse(int);
  Clauses isElectiveCourse(int);
  Clauses courseInMorningTime(int);
  Clauses programAtMostOneOfCoreOrElective(int);
  Clauses hasFieldTypeListedValues(int, FieldType, std::vector<int>);
};

#endif
