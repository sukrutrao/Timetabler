#include "constraint_encoder.h"

#include <vector>
#include <cassert>
#include <iostream>
#include "global.h"
#include "clauses.h"
#include "cclause.h"
#include "core/SolverTypes.h"
#include "time_tabler.h"

using namespace Minisat;

/**
 * @brief      Constructs the ConstraintEncoder object.
 *
 * @param      timeTabler  The time tabler
 */
ConstraintEncoder::ConstraintEncoder(TimeTabler *timeTabler) {
    this->timeTabler = timeTabler;
    this->vars = timeTabler->data.fieldValueVars;
}

/**
 * @brief      Gives Clauses that represent that a pair of courses have the
 *             same field value for a given FieldType.
 *             
 * For example, it can be used to represent that two courses have the same Slot.
 * This is helpful for constructing constraints such as those enforcing that
 * two courses have the same Slot.
 *
 * @param[in]  course1    The course 1
 * @param[in]  course2    The course 2
 * @param[in]  fieldType  The field type
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasSameFieldTypeAndValue(int course1, int course2, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        CClause field1, field2;
        field1.createLitAndAdd(vars[course1][fieldType][i]);
        field2.createLitAndAdd(vars[course2][fieldType][i]);
        Clauses conjunction(field1 & field2);
        if (i == 0) result = conjunction;
        else result = result | conjunction;
    }
    return result;
}

/**
 * @brief      Gives Clauses that represent that a pair of courses do not have the
 *             same field value for a given FieldType.
 *             
 * For example, it can be used to represent that two courses do not have the same
 * Instructor. This is helpful to define constraints such as that that an Instructor
 * cannot have two courses at the same time.
 *
 * @param[in]  course1    The course 1
 * @param[in]  course2    The course 2
 * @param[in]  fieldType  The field type
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasSameFieldTypeNotSameValue(int course1, int course2, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        CClause resultClause;
        resultClause.addLits(~mkLit(vars[course1][fieldType][i],false));
        resultClause.addLits(~mkLit(vars[course2][fieldType][i],false));
        result.addClauses(resultClause);
    }
    return result;
}

/**
 * @brief      Gives Clauses that represent that a pair of courses are
 *             core for a given Program.
 *
 * @param[in]  course1  The course 1
 * @param[in]  course2  The course 2
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasCommonProgram(int course1, int course2) {
    Clauses result;
    for(int i = 0; i < vars[course1][FieldType::program].size(); i++) {
        if(timeTabler->data.programs[i].isCoreProgram()) {
            CClause field1, field2;
            field1.createLitAndAdd(vars[course1][FieldType::program][i]);
            field2.createLitAndAdd(vars[course2][FieldType::program][i]);
            Clauses conjunction(field1 & field2);
            if (i == 0) result = conjunction;
            else result = result | conjunction;
        }
    }
    return result;
}

/**
 * @brief      Gives Clauses that represent that at least one out of a pair of courses
 *             is not core for a given Program.
 *             
 * This is helpful to represent that two courses that are core for a Program cannot
 * have an intersecting schedule.
 *
 * @param[in]  course1  The course 1
 * @param[in]  course2  The course 2
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasNoCommonCoreProgram(int course1, int course2) {
    Clauses result;
    for(int i = 0; i < vars[course1][FieldType::program].size(); i++) {
        if(timeTabler->data.programs[i].isCoreProgram()) {
            CClause resultClause;
            resultClause.addLits(~mkLit(vars[course1][FieldType::program][i],false));
            resultClause.addLits(~mkLit(vars[course2][FieldType::program][i],false));
            result.addClauses(resultClause);
        }
    }
    return result;
}

/**
 * @brief      Gives Clauses that represent that a pair of courses cannot have
 *             an intersecting schedule.
 *             
 * This describes that either the pair of courses have slots that do not intersect
 * or that they have segments that do not intersect.
 *
 * @param[in]  course1  The course 1
 * @param[in]  course2  The course 2
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::notIntersectingTime(int course1, int course2) {
    Clauses notSegmentIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::segment));
    Clauses notSlotIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::slot));
    return (notSegmentIntersecting | notSlotIntersecting);
}

/**
 * @brief      Gives Clauses that represent that a pair of courses cannot have
 *             an intersecting value for a given FieldType, where the FieldType is
 *             of a time field, which is either a Segment or a Slot.
 *
 * @param[in]  course1    The course 1
 * @param[in]  course2    The course 2
 * @param[in]  fieldType  The field type
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::notIntersectingTimeField(int course1, int course2, FieldType fieldType) {
    assert(fieldType == FieldType::segment || fieldType == FieldType::slot);
    assert(vars[course1][fieldType].size() == vars[course2][fieldType].size());
    assert(course1 != course2);
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        Clauses hasFieldValue1(vars[course1][fieldType][i]);
        Clauses notIntersecting1;
        for(int j = 0; j < vars[course1][fieldType].size(); j++) {
            if((fieldType == FieldType::segment && timeTabler->data.segments[i].isIntersecting(timeTabler->data.segments[j]))
                || (fieldType == FieldType::slot && timeTabler->data.slots[i].isIntersecting(timeTabler->data.slots[j]))) {
                notIntersecting1.addClauses(~Clauses(vars[course2][fieldType][j]));
            }
        }
        result.addClauses(hasFieldValue1>>notIntersecting1);
    }
    return result;
}

/**
 * @brief      Gives Clauses that represent that a Course can have exactly
 *             one field value of a given FieldType to be True.
 *             
 * This is helpful for defining constraints such as that that a given
 * Course must have exactly one Instructor.
 *
 * @param[in]  course     The course
 * @param[in]  fieldType  The field type
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasExactlyOneFieldValueTrue(int course, FieldType fieldType) {
    Clauses atLeastOne = Clauses(hasAtLeastOneFieldValueTrue(course, fieldType));
    Clauses atMostOne = Clauses(hasAtMostOneFieldValueTrue(course, fieldType));
    return (atLeastOne & atMostOne);
}

/**
 * @brief      Gives Clauses that represent that a Course can have
 *             at least one field value of a given FieldType to be True.
 *
 * @param[in]  course     The course
 * @param[in]  fieldType  The field type
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasAtLeastOneFieldValueTrue(int course, FieldType fieldType) {
    std::vector<Var> varsToUse = getAllowedVars(course, fieldType);
    CClause resultClause;
    for(int i = 0; i < varsToUse.size(); i++) {
        resultClause.createLitAndAdd(varsToUse[i]);
    }
    Clauses result(resultClause);
    return result;
}

/**
 * @brief      Gives Clauses that represent that a Course can have
 *             at most one field value of a given FieldType to be True.
 *             
 * The binomial encoding has been used to represent this constraint. A more
 * efficient encoding, with auxiliary variables, has not been used because
 * such an encoding would only be equisatisfiable. However, when using these
 * clauses as an antecedent of an implication, we need that if the antecedent
 * of the encoded clauses is False, the original set of clauses would also
 * have been False, a property that equisatifiability does not guarantee.
 *
 * @param[in]  course     The course
 * @param[in]  fieldType  The field type
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasAtMostOneFieldValueTrue(int course, FieldType fieldType) {
    std::vector<Var> varsToUse = getAllowedVars(course, fieldType);
    Clauses result;
    for(int i = 0; i < vars[course][fieldType].size(); i++) {
        for(int j = i+1; j < vars[course][fieldType].size(); j++) {
            Clauses first(vars[course][fieldType][i]);
            Clauses second(vars[course][fieldType][j]);
            Clauses negSecond = ~second;
            result.addClauses(~first | negSecond);
        }
    }
    return result;
}

/**
 * @brief      Gets a vector of Var that are to be considered for a given
 *             FieldType when defining constraints for that FieldType.
 *             
 * This is useful for the case of Classroom, where variables which represent
 * classrooms with size smaller than the class size of a Course are not
 * considered for that Course. For other FieldType values, all variables
 * are returned.
 *
 * @param[in]  course     The course
 * @param[in]  fieldType  The field type
 *
 * @return     The allowed variables.
 */
std::vector<Var> ConstraintEncoder::getAllowedVars(int course, FieldType fieldType) {
    std::vector<Var> varsToUse;
    varsToUse.clear();
    if(fieldType != FieldType::classroom) {
        varsToUse = vars[course][fieldType];
    }
    else {
        for(int i = 0; i < vars[course][fieldType].size(); i++) {
            if(timeTabler->data.courses[course].getClassSize() <= timeTabler->data.classrooms[i].getSize()) {
                varsToUse.push_back(vars[course][fieldType][i]);
            }
        }
    }
    return varsToUse;
}

/**
 * @brief      Gives Clauses that represent that a given Course is a minor Course.
 *
 * @param[in]  course  The course
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::isMinorCourse(int course) {
    Clauses result(vars[course][FieldType::isMinor][MinorType::isMinorCourse]);
    return result;
}

/**
 * @brief      Gives Clauses that represent that a given Course
 *             must have a minor Slot.
 *
 * @param[in]  course  The course
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::slotInMinorTime(int course) {
    CClause resultClause;
    for(int i = 0; i < vars[course][FieldType::slot].size(); i++) {
        if(timeTabler->data.slots[i].isMinorSlot()) {
            resultClause.createLitAndAdd(vars[course][FieldType::slot][i]);
        }
    }
    Clauses result(resultClause);
    return result;
}

/**
 * @brief      Gives Clauses that represent that a Course is a core Course.
 *
 * @param[in]  course  The course
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::isCoreCourse(int course) {
    CClause resultClause;
    for(int i = 0; i < vars[course][FieldType::program].size(); i++) {
        if(timeTabler->data.programs[i].isCoreProgram()) {
            resultClause.createLitAndAdd(vars[course][FieldType::program][i]);
        }
    }
    Clauses result(resultClause);
    return result;
}

/**
 * @brief      Gives Clauses that represent that a Course is an elective Course.
 *
 * @param[in]  course  The course
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::isElectiveCourse(int course) {
    CClause resultClause;
    for(int i = 0; i < vars[course][FieldType::program].size(); i++) {
        if(!(timeTabler->data.programs[i].isCoreProgram())) {
            resultClause.createLitAndAdd(vars[course][FieldType::program][i]);
        }
    }
    Clauses result(resultClause);
    return result;
}

/**
 * @brief      Gives Clauses that represent that a Course is
 *             scheduled in a morning Slot.
 *
 * @param[in]  course  The course
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::courseInMorningTime(int course) {
    CClause resultClause;
    for(int i = 0; i < vars[course][FieldType::slot].size(); i++) {
        if(timeTabler->data.slots[i].isMorningSlot()) {
            resultClause.createLitAndAdd(vars[course][FieldType::slot][i]);
        }
    }
    Clauses result(resultClause);
    return result;
}

/**
 * @brief      Gives Clauses that represent that for a Course, it can
 *             be a part of a Program as at most one out of core or
 *             elective.
 *
 * @param[in]  course  The course
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::programAtMostOneOfCoreOrElective(int course) {
    Clauses result;
    for(int i = 0; i < vars[course][FieldType::program].size(); i+=2) {
        CClause resultClause;
        resultClause.addLits(~mkLit(vars[course][FieldType::program][i], false));
        resultClause.addLits(~mkLit(vars[course][FieldType::program][i+1], false));
        result.addClauses(resultClause);
    }
    return result;
}

/**
 * @brief      Gives Clauses that represent that a Course has a field
 *             value for a given FieldType out of a list of possible values.
 *             
 * This is useful for encoding custom constraints.
 *
 * @param[in]  course     The course
 * @param[in]  fieldType  The field type
 * @param[in]  indexList  A vector of indices of the data corresponding to the 
 *                        FieldType for the allowed field values of the type.
 *
 * @return     A Clauses object representing the condition
 */
Clauses ConstraintEncoder::hasFieldTypeListedValues(int course, FieldType fieldType, std::vector<int> indexList) {
    CClause resultClause;
    for(int i = 0; i < indexList.size(); i++) {
        resultClause.createLitAndAdd(vars[course][fieldType][indexList[i]]);
    }
    Clauses result(resultClause);
    return result;
}