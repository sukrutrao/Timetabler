#include "constraint_adder.h"

ConstraintAdder::ConstraintAdder(Encoder encoder, TimeTabler *timeTabler) {
    this->encoder = encoder;
    this->timeTabler = timeTabler;
}

Clauses ConstraintAdder::fieldSingleValueAtATime(FieldType fieldType) {
    Clauses result;
    result.clear();
    Course courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        for(int j = i+1; j < courses.size(); j++) {
            Clauses antecedent = encoder->hasFieldTypeAndValue(i, j, fieldType);
            Clauses consequent = encoder->notIntersectingTime(i, j);
            result.addClauses(antecedent->consequent);
        }
    }
    return result;
}

Clauses ConstraintAdder::instructorSingleCourseAtATime() {
    return fieldSingleValueAtATime(FieldType::instructor);
}

Clauses ConstraintAdder::classroomSingleCourseAtATime() {
    return fieldSingleValueAtATime(FieldType::classroom);
}

Clauses ConstraintAdder::programSingleCoreCourseAtATime() {
    return fieldSingleValueAtATime(FieldType::program); // TODO - check, program is actually a list
    // even == has not been defined yet
}

Clauses ConstraintAdder::minorInMinorTime() {
    Clauses result;
    result.clear();
}

