#include "constraint_adder.h"

#include <vector>
#include "constraint_encoder.h"
#include "time_tabler.h"
#include "global.h"
#include "clauses.h"
#include "core/SolverTypes.h"
#include "utils.h"
#include <iostream>

using namespace Minisat;

// TODO - we should define high level variables for each course

ConstraintAdder::ConstraintAdder(ConstraintEncoder *encoder, TimeTabler *timeTabler) {
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
            // std::cout << "Course " << i << ", " << j << " : " << std::endl;
            antecedent.print();
            consequent.print();
            Clauses r = antecedent>>consequent;
            r.print();
            result.addClauses(r);

        }
    }
    // result.print();
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

Clauses ConstraintAdder::exactlyOneFieldValuePerCourse(FieldType fieldType) {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses exactlyOneFieldValue = encoder->hasExactlyOneFieldValueTrue(i, fieldType);
        // std::cout << "Course " << i << " : " << std::endl;
        // exactlyOneFieldValue.print();
        Clauses cclause(timeTabler->data.highLevelVars[i][fieldType]);
     /*   Clauses negateCClause = ~cclause;
        Clauses rhs = ~exactlyOneFieldValue | cclause;
        result.addClauses((exactlyOneFieldValue | negateCClause) & rhs);*/
        Clauses first = cclause>>exactlyOneFieldValue;
   //     Clauses second = exactlyOneFieldValue>>cclause;
        result.addClauses(first);// & second);
    }
    return result;
}

Clauses ConstraintAdder::existingAssignmentClausesSoft() {
    Clauses result;
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses thisCourse = encoder->existingAssignments(i);
        result.addClauses(thisCourse);
    }
    return result;
}

// Clauses ConstraintAdder::exactlyOneTimePerCourse() {
//     Clauses result;
//     result.clear();
//     std::vector<Course> courses = timeTabler->data.courses;
//     for(int i = 0; i < courses.size(); i++) {
//         Clauses exactlyOneSlot = encoder->hasExactlyOneFieldValueTrue(i, FieldType::slot);
//         result.addClauses(exactlyOneSlot);
//     }
//     return result;
// }

// Clauses ConstraintAdder::exactlyOneClassroomPerCourse() {
//     Clauses result;
//     result.clear();
//     std::vector<Course> courses = timeTabler->data.courses;
//     for(int i = 0; i < courses.size(); i++) {
//         Clauses exactlyOneClassroom = encoder->hasExactlyOneFieldValueTrue(i, FieldType::classroom);
//         result.addClauses(exactlyOneClassroom);
//     }
//     return result;
// }

Clauses ConstraintAdder::addConstraints() {
    Clauses result;
    result.clear();
    // TODO - need to define high level variables here
    result.addClauses(instructorSingleCourseAtATime());
    result.addClauses(classroomSingleCourseAtATime());
    result.addClauses(programSingleCoreCourseAtATime());
    result.addClauses(minorInMinorTime());
    // result.addClauses(exactlyOneTimePerCourse());
    // result.addClauses(exactlyOneClassroomPerCourse());

    // Clauses r = exactlyOneFieldValuePerCourse(FieldType::slot);
    // r.print();
    // result.addClauses(r);

    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::slot));
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::classroom));
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::instructor));
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::isMinor));
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::segment));
    // TODO Add clauses to timeTabler Solver

    return result;
}

Clauses ConstraintAdder::softConstraints() {
    return existingAssignmentClausesSoft();
}

Clauses ConstraintAdder::coreInMorningTime() {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses coreCourse = encoder->isCoreCourse(i);
        Clauses morningTime = encoder->courseInMorningTime(i);
        result.addClauses(coreCourse>>morningTime); // TODO - have to do the converse as well, but with lower priority (weight)
    }
    return result;
}