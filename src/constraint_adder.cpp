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
            Clauses antecedent = encoder->hasSameFieldTypeNotSameValue(i, j, fieldType);
            Clauses consequent = encoder->notIntersectingTime(i, j);
            // std::cout << "Course " << i << ", " << j << " : " << std::endl;
        //    antecedent.print();
        //    consequent.print();
            Clauses r = antecedent | consequent;
        //    r.print();
            result.addClauses(r);

        }
    }
    std::cout << "Result size : " << result.getClauses().size() << std::endl;
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
            Clauses antecedent = encoder->hasNoCommonCoreProgram(i, j);
            std::cout << "HCP : " << antecedent.getClauses().size() << "\n";
            Clauses consequent = encoder->notIntersectingTime(i, j);
            std::cout << "NIT : " << consequent.getClauses().size() << "\n";
            Clauses r = antecedent | consequent;
            std::cout << "R\n";
            result.addClauses(r);
        }
    }
    std::cout << "Ret\n";
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

/*Clauses ConstraintAdder::existingAssignmentClauses() {
    Clauses result;
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        Clauses thisCourse = encoder->existingAssignments(i);
        result.addClauses(thisCourse);
    }
    return result;
}*/

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

void ConstraintAdder::addConstraints() {
    Clauses result;
    result.clear();
    std::vector<int> weights = timeTabler->data.predefinedClausesWeights;
    // TODO - need to define high level variables here
   // std::cout << "-3\n";
    timeTabler->addClauses(instructorSingleCourseAtATime(), weights[PredefinedClauses::instructorSingleCourseAtATime]);
    timeTabler->addClauses(classroomSingleCourseAtATime(), weights[PredefinedClauses::classroomSingleCourseAtATime]);
    timeTabler->addClauses(programSingleCoreCourseAtATime(), weights[PredefinedClauses::programSingleCoreCourseAtATime]);
    timeTabler->addClauses(minorInMinorTime(), weights[PredefinedClauses::minorInMinorTime]);

    timeTabler->addClauses(exactlyOneFieldValuePerCourse(FieldType::slot), weights[PredefinedClauses::exactlyOneSlotPerCourse]);
    std::cout << "WEIGHT CLASSROOM : " << weights[PredefinedClauses::exactlyOneInstructorPerCourse] << std::endl;
    exactlyOneFieldValuePerCourse(FieldType::classroom).print();
    timeTabler->addClauses(exactlyOneFieldValuePerCourse(FieldType::classroom), weights[PredefinedClauses::exactlyOneClassroomPerCourse]);
    timeTabler->addClauses(exactlyOneFieldValuePerCourse(FieldType::instructor), weights[PredefinedClauses::exactlyOneInstructorPerCourse]);
    timeTabler->addClauses(exactlyOneFieldValuePerCourse(FieldType::isMinor), weights[PredefinedClauses::exactlyOneIsMinorPerCourse]);
    timeTabler->addClauses(exactlyOneFieldValuePerCourse(FieldType::segment), weights[PredefinedClauses::exactlyOneSegmentPerCourse]);

    timeTabler->addClauses(coreInMorningTime(), weights[PredefinedClauses::coreInMorningTime]);

 //   timeTabler->addClauses(programAtMostOneOfCoreOrElective(), weights[PredefinedClauses::programAtMostOneOfCoreOrElective]);

   /* result.addClauses(instructorSingleCourseAtATime());
    std::cout << "-2\n";
    result.addClauses(classroomSingleCourseAtATime());
    std::cout << "-1\n";
    result.addClauses(programSingleCoreCourseAtATime());
    std::cout << "0\n";
    result.addClauses(minorInMinorTime());
    // result.addClauses(exactlyOneTimePerCourse());
    // result.addClauses(exactlyOneClassroomPerCourse());

    // Clauses r = exactlyOneFieldValuePerCourse(FieldType::slot);
    // r.print();
    // result.addClauses(r);
    std::cout << "1\n";
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::slot));
    std::cout << "2\n";
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::classroom));
    std::cout << "3\n";
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::instructor));
    std::cout << "4\n";
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::isMinor));
    std::cout << "5\n";
    result.addClauses(exactlyOneFieldValuePerCourse(FieldType::segment));
    std::cout << "6\n";*/
    // TODO Add clauses to timeTabler Solver
//    return result;
}

/*Clauses ConstraintAdder::softConstraints() {
    return existingAssignmentClausesSoft();
}*/

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

Clauses ConstraintAdder::programAtMostOneOfCoreOrElective() {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        result.addClauses(encoder->programAtMostOneOfCoreOrElective(i));
    }
    return result;
}

Clauses ConstraintAdder::customConstraint(FieldType fieldLHS, std::vector<int> list1, FieldType classroomOrSlot, std::vector<int> list2, bool isNegated) {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    if(fieldLHS == FieldType::instructor) {
        std::cout << "instructor\n";
        for(int i = 0; i < list1.size(); i++) {
            std::cout << timeTabler->data.instructors[list1[i]].getName() << std::endl;
        }
    }
    if(classroomOrSlot == FieldType::classroom) {
        std::cout << "classroom\n";
        for(int i = 0; i < list2.size(); i++) {
            std::cout << timeTabler->data.classrooms[list2[i]].getName() << std::endl;
        }
    }
    std::cout << isNegated << "\n";
    for(int i = 0; i < courses.size(); i++) {
        Clauses antecedent = encoder->hasFieldTypeListedValues(i, fieldLHS, list1);
        Clauses consequent = encoder->hasFieldTypeListedValues(i, classroomOrSlot, list2);
        consequent.print();
        if(isNegated) {
            std::cout << "NEGATED!!!\n";
            consequent = ~consequent;
        }
        result.addClauses(antecedent>>consequent);
    }
    result.print();
    return result;
}

Clauses ConstraintAdder::customConstraint(std::vector<int> courseList, FieldType classroomOrSlot, std::vector<int> listOfValues, bool isNegated) {
    Clauses result;
    result.clear();
    for(int i = 0; i < courseList.size(); i++) {
        Clauses clause = encoder->hasFieldTypeListedValues(courseList[i], classroomOrSlot, listOfValues);
        if(isNegated) {
            clause = ~clause;
        }
        result.addClauses(clause);
    }
    return result;
}