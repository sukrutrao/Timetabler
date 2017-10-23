#ifndef CONSTRAINT_ENCODER_H
#define CONSTRAINT_ENCODER_H

#include <vector>
#include "global.h"
#include "clauses.h"
#include "core/SolverTypes.h"
#include "time_tabler.h"

using namespace Minisat;

/**
 * @brief      Class for constraint encoder.
 * 
 * This class is responsible for imposing constraints given a Course
 * or a pair of Courses and a FieldType. The constraints are of a higher
 * level than the operations of Clauses and of a lower level than
 * the constraints defined in ConstraintAdder, which are the final constraints
 * added to the solver. The class keeps a copy of the variables used
 * for every 3-tuple of (Course, FieldType, field value) and a pointer to
 * a TimeTabler object to access the field data. Clauses for these constraints
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
     * A pointer to a TimeTabler object for accessing field data
     */
    TimeTabler *timeTabler;
    std::vector<Var> getAllowedVars(int, FieldType);
public:
    ConstraintEncoder(TimeTabler*);
//  Clauses createClauses(int, int, std::function<bool(int, int, FieldType)>);
    Clauses hasSameFieldTypeAndValue(int, int, FieldType);
    Clauses hasSameFieldTypeNotSameValue(int, int, FieldType);
    Clauses hasSameFieldType(int, int, FieldType); // is this needed?
    Clauses notIntersectingTime(int, int);
    Clauses notIntersectingTimeField(int, int, FieldType);
    Clauses hasExactlyOneFieldValueTrue(int, FieldType);
    Clauses hasAtLeastOneFieldValueTrue(int, FieldType);
    Clauses hasAtMostOneFieldValueTrue(int, FieldType);
    Clauses hasCommonProgram(int, int);
    Clauses hasNoCommonCoreProgram(int, int);
    Clauses isMinorCourse(int);
    Clauses slotInMinorTime(int);
    /*Clauses existingAssignments(int);*/
    Clauses isCoreCourse(int);
    Clauses courseInMorningTime(int);
    Clauses programAtMostOneOfCoreOrElective(int);
    Clauses hasFieldTypeListedValues(int, FieldType, std::vector<int>);
};

#endif