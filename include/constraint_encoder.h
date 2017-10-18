#ifndef CONSTRAINT_ENCODER_H
#define CONSTRAINT_ENCODER_H

#include <vector>
#include "global.h"
#include "clauses.h"
#include "core/SolverTypes.h"
#include "time_tabler.h"

using namespace Minisat;

class ConstraintEncoder {
private:
    std::vector<std::vector<std::vector<Var>>> vars;
    TimeTabler *timeTabler;
public:
    ConstraintEncoder(std::vector<std::vector<std::vector<Var>>>);
    ConstraintEncoder(TimeTabler*);
//  Clauses createClauses(int, int, std::function<bool(int, int, FieldType)>);
    Clauses hasSameFieldTypeAndValue(int, int, FieldType);
    Clauses hasFieldType(int, FieldType); // is this needed?
    Clauses hasSameFieldType(int, int, FieldType); // is this needed?
    Clauses notIntersectingTime(int, int);
    Clauses notIntersectingTimeField(int, int, FieldType);
    Clauses hasExactlyOneFieldValueTrue(int, FieldType);
    Clauses hasAtLeastOneFieldValueTrue(int, FieldType);
    Clauses hasAtMostOneFieldValueTrue(int, FieldType);
    Clauses hasCommonProgram(int, int);
    Clauses isMinorCourse(int);
    Clauses slotInMinorTime(int);
};

#endif