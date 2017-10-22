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

ConstraintEncoder::ConstraintEncoder(std::vector<std::vector<std::vector<Var>>> vars) {
    this->vars = vars;
}

ConstraintEncoder::ConstraintEncoder(TimeTabler *timeTabler) {
    this->timeTabler = timeTabler;
    this->vars = timeTabler->data.fieldValueVars;
}

Clauses ConstraintEncoder::hasSameFieldTypeAndValue(int course1, int course2, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        /*if(fieldType == FieldType::classroom) {
            std::cout << "FIELDTYPECLASSROOM:::::::\n";
            std::cout << timeTabler->data.courses[course1].getName() << " " << timeTabler->data.courses[course2].getName() << "\n";
            std::cout << timeTabler->data.classrooms[i].getName() << "\n";
        }*/
        CClause field1, field2;
        field1.createLitAndAdd(vars[course1][fieldType][i]);
        field2.createLitAndAdd(vars[course2][fieldType][i]);
        Clauses conjunction(field1 & field2);
        if (i == 0) result = conjunction;
        else result = result | conjunction;
    }
    result.print();
    return result;
}

Clauses ConstraintEncoder::hasSameFieldTypeNotSameValue(int course1, int course2, FieldType fieldType) {
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        CClause resultClause;
        resultClause.addLits(~mkLit(vars[course1][fieldType][i],false));
        resultClause.addLits(~mkLit(vars[course2][fieldType][i],false));
        result.addClauses(resultClause);
    }
    result.print();
    return result;
}

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

Clauses ConstraintEncoder::hasNoCommonCoreProgram(int course1, int course2) {
    Clauses result;
    for(int i = 0; i < vars[course1][FieldType::program].size(); i++) {
        if(!timeTabler->data.programs[i].isCoreProgram()) {
            CClause resultClause;
            resultClause.addLits(~mkLit(vars[course1][FieldType::program][i],false));
            resultClause.addLits(~mkLit(vars[course2][FieldType::program][i],false));
            result.addClauses(resultClause);
        }
    }
    result.print();
    return result;
}

Clauses ConstraintEncoder::notIntersectingTime(int course1, int course2) {
    Clauses notSegmentIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::segment));
    Clauses notSlotIntersecting = Clauses(notIntersectingTimeField(course1, course2, FieldType::slot));
    return (notSegmentIntersecting | notSlotIntersecting);
}

Clauses ConstraintEncoder::notIntersectingTimeField(int course1, int course2, FieldType fieldType) {
    assert(fieldType == FieldType::segment || fieldType == FieldType::slot);
    assert(vars[course1][fieldType].size() == vars[course2][fieldType].size());
    assert(course1 != course2);
    Clauses result;
    for(int i = 0; i < vars[course1][fieldType].size(); i++) {
        Clauses hasFieldValue1(vars[course1][fieldType][i]);
        // Clauses hasFieldValue2(vars[course2][fieldType][i]);
        Clauses notIntersecting1;
        // Clauses notIntersecting2;
        for(int j = 0; j < vars[course1][fieldType].size(); j++) {
      /*      if(i == j) {
                continue;
            }*/ // TODO - can we have this?
            if((fieldType == FieldType::segment && timeTabler->data.segments[i].isIntersecting(timeTabler->data.segments[j]))
                || (fieldType == FieldType::slot && timeTabler->data.slots[i].isIntersecting(timeTabler->data.slots[j]))) {
                notIntersecting1.addClauses(~Clauses(vars[course2][fieldType][j]));
                // notIntersecting2.addClauses(~Clauses(vars[course1][fieldType][j]));
            //    std::cout << "CN : " << course1 << " " << course2 << std::endl;
             //   std::cout << timeTabler->data.courses[course1].getName() << " " << timeTabler->data.courses[course2].getName() << std::endl;
         /*       if(fieldType == FieldType::segment) {
                    std::cout << "1 : " << timeTabler->data.segments[i].toString();
                    std::cout << " 2 : " << timeTabler->data.segments[j].toString() << std::endl;   
                }
                if(fieldType == FieldType::slot) {
                    std::cout << "1 : " << timeTabler->data.slots[i].getName();
                    std::cout << " 2 : " << timeTabler->data.slots[j].getName() << std::endl;
                }*/
            }
        }
        Clauses r = hasFieldValue1>>notIntersecting1;
        result.addClauses(r);
        // result.addClauses(hasFieldValue2>>notIntersecting2);
    }
    return result;
}

Clauses ConstraintEncoder::hasExactlyOneFieldValueTrue(int course, FieldType fieldType) {
    Clauses atLeastOne = Clauses(hasAtLeastOneFieldValueTrue(course, fieldType));
    Clauses atMostOne = Clauses(hasAtMostOneFieldValueTrue(course, fieldType));
    return (atLeastOne & atMostOne);
}

Clauses ConstraintEncoder::hasAtLeastOneFieldValueTrue(int course, FieldType fieldType) {
    std::vector<Var> varsToUse = getAllowedVars(course, fieldType);
    // TODO - check that an empty clause is actually created. This can be easily verified if we give input so that varsToUse is empty
    CClause resultClause;
    for(int i = 0; i < varsToUse.size(); i++) {
        resultClause.createLitAndAdd(varsToUse[i]);
    }
    Clauses result(resultClause);
    return result;
}

// TODO - incomplete - who is responsible for add Var s to the solver?
// TODO - use a good encoding, using binomial for now
Clauses ConstraintEncoder::hasAtMostOneFieldValueTrue(int course, FieldType fieldType) {
  /*  int size = vars[course][fieldType].size();
    std::vector<Var> encoderVars = */
    std::vector<Var> varsToUse = getAllowedVars(course, fieldType);
    Clauses result;
    // Sequential encoding
    std::vector<Lit> s;
    int n = varsToUse.size();
    // TODO : check this
    if(n <= 1) {
        result.addClauses(CClause(timeTabler->newVar())); // just a new dummy so that clause is not empty
        return result;
    }
    for (int i=0; i<n-1; i++) {
        Var v = timeTabler->newVar();
        s.push_back(mkLit(v, false));
    }
    Clauses x1 = ~CClause(varsToUse[0]);
    result.addClauses(CClause(s[0]) | x1);
    Clauses xn = ~CClause(varsToUse[n-1]);
    result.addClauses(Clauses(~CClause(s[n-2])) | xn);
    for (int i=1; i<n-1; i++) {
        Clauses xi = ~CClause(varsToUse[i]);
        CClause si = CClause(s[i]);
        result.addClauses(CClause(s[i]) | xi);
        result.addClauses(Clauses(~CClause(s[i-1])) | si);
        result.addClauses(Clauses(~CClause(s[i-1])) | xi);
    }
  /*  for(int i = 0; i < vars[course][fieldType].size(); i++) {
        for(int j = i+1; j < vars[course][fieldType].size(); j++) {
            Clauses first(vars[course][fieldType][i]);
            Clauses second(vars[course][fieldType][j]);
            Clauses negSecond = ~second;
            result.addClauses(~first | negSecond);
        }
    }*/
    return result;
}

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

Clauses ConstraintEncoder::hasFieldType(int course, FieldType fieldType) {
    CClause resultClause;
    for(int i = 0; i < vars[course][fieldType].size(); i++) {
        resultClause.createLitAndAdd(vars[course][fieldType][i]);
    }
    Clauses result(resultClause);
    return result;
}

Clauses ConstraintEncoder::isMinorCourse(int course) {
    Clauses result(vars[course][FieldType::isMinor][MinorType::isMinorCourse]);
    return result;
}

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

Clauses ConstraintEncoder::existingAssignments(int course) {
    Clauses result;
    for(int i = 0; i < vars[course][FieldType::instructor].size(); i++) {
        Lit p = mkLit(vars[course][FieldType::instructor][i], false);
        if(timeTabler->data.courses[course].getInstructor() == i) {
            result.addClauses(Clauses(p));
        }
        else {
            result.addClauses(Clauses(~p));
        }
    }
    for(int i = 0; i < vars[course][FieldType::segment].size(); i++) {
        Lit p = mkLit(vars[course][FieldType::segment][i], false);
        if(timeTabler->data.courses[course].getSegment() == i) {
            result.addClauses(Clauses(p));
        }
        else {
            result.addClauses(Clauses(~p));
        }
    }
    for(int i = 0; i < vars[course][FieldType::isMinor].size(); i++) {
        Lit p = mkLit(vars[course][FieldType::isMinor][i], false);
        if(timeTabler->data.courses[course].getIsMinor() == i) {
            result.addClauses(Clauses(p));
        }
        else {
            result.addClauses(Clauses(~p));
        }
    }
    for(int i = 0; i < vars[course][FieldType::program].size(); i++) {
        Lit p = mkLit(vars[course][FieldType::program][i], false);
        std::vector<int> programsThisCourse = timeTabler->data.courses[course].getPrograms();
        if(std::find(programsThisCourse.begin(), programsThisCourse.end(), i) != programsThisCourse.end()) {
            result.addClauses(Clauses(p));
        }
        else {
            result.addClauses(Clauses(~p));
        }
    }
    return result;
}

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
