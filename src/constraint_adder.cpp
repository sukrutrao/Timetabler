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

/**
 * @brief      Constructs the ConstraintAdder object.
 *
 * @param      encoder     The encoder
 * @param      timeTabler  The time tabler
 */
ConstraintAdder::ConstraintAdder(ConstraintEncoder *encoder, TimeTabler *timeTabler) {
    this->encoder = encoder;
    this->timeTabler = timeTabler;
}

/**
 * @brief      Imposes the constraint that a given FieldType value should be true for
 *             at most one Course at a time.
 *             
 * For example, this includes constraints such as enforcing that a given Instructor
 * cannot have two courses at the same time. Here, a time refers to a combination
 * of segment and slot. This is not added directly, but called by other functions.
 *
 * @param[in]  fieldType  The field type on which this constraint is imposed
 *
 * @return     A Clauses object describing this constraint
 */
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

/**
 * @brief      Imposes the constraint that an Instructor can have only a single
 *             course at a given time.
 *             
 * This simply calls fieldSingleValueAtATime with the FieldType as FieldType::instructor.
 * By default, this constraint is hard.
 *
 * @return     A Clauses object describing this constraint
 */
Clauses ConstraintAdder::instructorSingleCourseAtATime() {
    return fieldSingleValueAtATime(FieldType::instructor);
}

/**
 * @brief      Imposes the constraint that a Classroom can have only a single
 *             course at a given time.
 *             
 * This simply calls fieldSingleValueAtATime with the FieldType as FieldType::classroom.
 * By default, this constraint is hard.
 *
 * @return     A Clauses object describing this constraint
 */
Clauses ConstraintAdder::classroomSingleCourseAtATime() {
    return fieldSingleValueAtATime(FieldType::classroom);
}

/**
 * @brief      Imposes the constraint that if two courses are core for
 *             a Program, then they are not scheduled at an intersecting
 *             time.
 *             
 * By default, this constraint is hard.
 *
 * @return     A Clauses object describing the constraint
 */
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

/**
 * @brief      Imposes the constraint that a minor Course must be
 *             scheduled in a minor slot.
 *             
 * By default, this constraint is hard.
 *
 * @return     A Clauses object describing the constraint
 */
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

/**
 * @brief      Imposes the constraint that the given FieldType has exactly one value
 *             True for a given Course.
 *             
 * For example, it imposes the constraint that a given Course has exactly one Instructor.
 * By default, these constraints are hard. However, there exist high level
 * variables to notify the user if a particular constraint in this category could
 * not be satisfied, in order to help in making the necessary modifications.
 *
 * @param[in]  fieldType  The field type on which this constraint is imposed
 *
 * @return     A Clauses object describing the constraint
 */
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


/**
 * @brief      Adds all the constraints with their respective weights using the TimeTabler object
 *             to the solver.
 */
void ConstraintAdder::addConstraints() {
    /*Clauses result;
    result.clear();*/
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

/**
 * @brief      Imposes that a core course is given a morning slot.
 * 
 * By default, this constraint is soft with weight 1.
 *
 * @return     A Clauses object describing the constraint
 */
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

/**
 * @brief      Imposes the constraint that for a course, a program can either
 *             be a core program or an elective program, or neither, but not both.
 *             
 * By default, this constraint is hard.
 *
 * @return     A Clauses object describing the constraint
 */
Clauses ConstraintAdder::programAtMostOneOfCoreOrElective() {
    Clauses result;
    result.clear();
    std::vector<Course> courses = timeTabler->data.courses;
    for(int i = 0; i < courses.size(); i++) {
        result.addClauses(encoder->programAtMostOneOfCoreOrElective(i));
    }
    return result;
}

/*Clauses ConstraintAdder::customConstraint(FieldType fieldLHS, std::vector<int> list1, FieldType classroomOrSlot, std::vector<int> list2, bool isNegated) {
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
}*/