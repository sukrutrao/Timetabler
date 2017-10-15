#include "constraint_adder.h"

#include <vector>
#include "encoder.h"
#include "time_tabler.h"
#include "global.h"
#include "clauses.h"

ConstraintAdder::ConstraintAdder(Encoder encoder, TimeTabler *timeTabler) {
    this->encoder = encoder;
    this->timeTabler = timeTabler;
}

Clauses ConstraintAdder::fieldSingleValueAtATime(FieldType fieldType) {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        for(int j = i+1; j < courses.size(); j++) {
            Clauses antecedent = encoder->hasSameFieldTypeAndValue(i, j, fieldType);
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
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses antecedent = encoder->isMinorCourse(i);
        Clauses consequent = encoder->slotInMinorTime(i);
        result.addClauses(antecedent->consequent);
        result.addClauses(consequent->antecedent);
    }
    return result;
}

Clauses ConstraintAdder::exactlyOneTimePerCourse() {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses exactlyOneSlot = encoder->hasExactlyOneFieldValueTrue(i, FieldType::slot);
        result.addClauses(exactlyOneSlot);
    }
    return result;
}

Clauses ConstraintAdder::exactlyOneClassroomPerCourse() {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses exactlyOneClassroom = encoder->hasExactlyOneFieldValueTrue(i, FieldType::classroom);
        result.addClauses(exactlyOneClassroom);
    }
    return result;
}