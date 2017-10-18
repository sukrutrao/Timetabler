#include "encoder.h"

#include <vector>
#include <cassert>
#include "global.h"
#include "clauses.h"
#include "cclause.h"
#include "core/SolverTypes.h"
#include "time_tabler.h"

using namespace Minisat;

Encoder::Encoder(std::vector<std::vector<std::vector<Var>>> vars) {
    this->vars = vars;
}

Encoder::Encoder(TimeTabler *timeTabler) {
    this->timeTabler = timeTabler;
    this->vars = timeTabler->data.fieldValueVars;
}

Clauses Encoder::hasSameFieldTypeAndValue(int course1, int course2, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        CClause field1, field2;
        field1.createLitAndAdd(vars[course1][fieldType][i]);
        field2.createLitAndAdd(vars[course1][fieldType][i]);
        result.addClauses(field1 & field2);
    }
    return result;
}

Clauses Encoder::hasCommonProgram(int course1, int course2) {
    Clauses result;
    for(int i = 0; i < vars[course1][FieldType::program].size(); i++) {
        if(timeTabler->data.programs[i].isCoreProgram()) {
            CClause field1, field2;
            field1.createLitAndAdd(vars[course1][FieldType::program][i]);
            field2.createLitAndAdd(vars[course1][FieldType::program][i]);
            result.addClauses(field1 & field2);
        }
    }
    return result;
}

Clauses Encoder::notIntersectingTime(int course1, int course2) {
    Clauses notSegmentIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::segment));
    Clauses notSlotIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::slot));
    return (notSegmentIntersecting | notSlotIntersecting);
}

Clauses Encoder::notIntersectingTimeField(int course1, int course2, FieldType fieldType) {
    assert(fieldType == FieldType::segment || fieldType == FieldType::slot);
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        Clauses hasFieldValue1(vars[course1][fieldType][i]);
        Clauses hasFieldValue2(vars[course2][fieldType][i]);
        Clauses notIntersecting1;
        Clauses notIntersecting2;
        for(int j = 0; j < vars[course1][fieldType].size(); j++) {
            if(i == j) {
                continue;
            }
            if((fieldType == FieldType::segment && timeTabler->data.segments[i].isIntersecting(timeTabler->data.segments[j]))
                || (fieldType == FieldType::slot && timeTabler->data.slots[i].isIntersecting(timeTabler->data.slots[j]))) {
                notIntersecting1.addClauses(~Clauses(vars[course2][fieldType][j]));
                notIntersecting2.addClauses(~Clauses(vars[course1][fieldType][j]));
            }
        }
        result.addClauses(hasFieldValue1>>notIntersecting1);
        result.addClauses(hasFieldValue2>>notIntersecting2);
    }
    return result;
}

Clauses Encoder::hasExactlyOneFieldValueTrue(int course, FieldType fieldType) {
    Clauses atLeastOne = Clauses(hasAtLeastOneFieldValueTrue(course, fieldType));
    Clauses atMostOne = Clauses(hasAtMostOneFieldValueTrue(course, fieldType));
    return (atLeastOne & atMostOne);
}

Clauses Encoder::hasAtLeastOneFieldValueTrue(int course, FieldType fieldType) {
    CClause resultClause;
    for(int i = 0; i < vars[course][fieldType].size(); i++) {
        resultClause.createLitAndAdd(vars[course][fieldType][i]);
    }
    Clauses result(resultClause);
    return result;
}

// TODO - incomplete - who is responsible for add Var s to the solver?
// TODO - use a good encoding, using binomial for now
Clauses Encoder::hasAtMostOneFieldValueTrue(int course, FieldType fieldType) {
  /*  int size = vars[course][fieldType].size();
    std::vector<Var> encoderVars = */
    Clauses result;
    for(int i = 0; i < vars[course][fieldType].size(); i++) {
        for(int j = i+1; j < vars[course][fieldType].size(); j++) {
            Clauses first(vars[course][fieldType][i]);
            Clauses second(vars[course][fieldType][j]);
            result.addClauses(first | second);
        }
    }
    return result;
}

Clauses Encoder::hasFieldType(int course, FieldType fieldType) {
    CClause resultClause;
    for(int i = 0; i < vars[course][fieldType].size(); i++) {
        resultClause.createLitAndAdd(vars[course][fieldType][i]);
    }
    Clauses result(resultClause);
    return result;
}

Clauses Encoder::isMinorCourse(int course) {
    Clauses result(vars[course][FieldType::isMinor][MinorType::isMinorCourse]);
    return result;
}

Clauses Encoder::slotInMinorTime(int course) {
    CClause resultClause;
    for(int i = 0; i < vars[course][FieldType::slot].size(); i++) {
        if(timeTabler->data.slots[i].isMinorSlot()) {
            resultClause.createLitAndAdd(vars[course][FieldType::slot][i]);
        }
    }
    Clauses result(resultClause);
    return result;
}