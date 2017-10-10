#include "encoder.h"

#include <vector>
#include "global.h"

Encoder::Encoder(std::vector<std::vector<std::vector<Var>>> vars, const int inputCourseCount, 
            const int inputFieldCount) : courseCount(inputCourseCount), fieldCount(inputFieldCount) {
    this->vars = vars;
}

Clauses Encoder::hasSameFieldTypeAndValue(int course1, int course2, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        CClause field1, field2;
        field1.createLitAndAdd(vars[course1][fieldType][i]);
        field2.createLitAndAdd(vars[course1][fieldType][i]);
        result = result | (field1 & field2);
    }
    return result;
}

// TODO - what is the utility of this?
Clauses Encoder::hasFieldType(int course, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course][fieldType].size(); i++) {
        CClause field(vars[course][fieldType][i]);
        result = result | field;
    }
    return result;
}

Clauses Encoder::notIntersectingTime(int course1, int course2) {
    Clauses notSegmentIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::segment));
    Clauses notSlotIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::slot));
    return (notSegmentIntersecting | notSlotIntersecting);
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
Clauses Encoder::hasAtMostOneFieldValueTrue(int course, FieldType fieldType) {
    int size = vars[course][fieldType].size();
    std::vector<Var> encoderVars = //
}