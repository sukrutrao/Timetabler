#include "constraint_adder.h"

#include <vector>
#include "encoder.h"
#include "time_tabler.h"
#include "global.h"
#include "clauses.h"

#include "encoder.h"
#include "clauses.h"
#include "time_tabler.h"
#include "global.h"
#include "core/SolverTypes.h"

using namespace Minisat;

// TODO - we should define high level variables for each course

ConstraintAdder::ConstraintAdder(Encoder *encoder, TimeTabler *timeTabler) {
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
            result.addClauses(antecedent>>consequent);
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
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        for(int j = i+1; j < courses.size(); j++) {
            Clauses antecedent = encoder->hasCommonProgram(i, j);
            Clauses consequent = encoder->notIntersectingTime(i, j);
            result.addClauses(antecedent>>consequent);
        }
    }
    return result;
}

Clauses ConstraintAdder::minorInMinorTime() {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses antecedent = encoder->isMinorCourse(i);
        Clauses consequent = encoder->slotInMinorTime(i);
        result.addClauses(antecedent>>consequent);
        result.addClauses(consequent>>antecedent);
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

Clauses ConstraintAdder::addConstraints() {
    Clauses result;
    // TODO - need to define high level variables here
    result.addClauses(instructorSingleCourseAtATime());
    result.addClauses(classroomSingleCourseAtATime());
    result.addClauses(programSingleCoreCourseAtATime());
    result.addClauses(minorInMinorTime());
    result.addClauses(exactlyOneTimePerCourse());
    result.addClauses(exactlyOneClassroomPerCourse());
    return result;
}