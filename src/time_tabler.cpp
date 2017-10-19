#include "time_tabler.h"

#include <vector>
#include <iostream>
#include "cclause.h"
#include "mtl/Vec.h"
#include "core/SolverTypes.h"
#include "tsolver.h"
#include "utils.h"
#include "MaxSATFormula.h"
#include "clauses.h"

using namespace Minisat;

TimeTabler::TimeTabler() {
    solver = new TSolver(_VERBOSITY_MINIMAL_, _CARD_TOTALIZER_);
    formula = new MaxSATFormula();
}

void TimeTabler::addClauses(std::vector<CClause> clauses) {
    vec<vec<Lit>> clausesVec;
    std::cout << "Clause count : " << clauses.size() << std::endl;
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clauseVec;
        std::vector<Lit> clauseVector = clauses[i].getLits();
        for(int j = 0; j < clauseVector.size(); j++) {
            clauseVec.push(clauseVector[j]);
        }
        formula->addHardClause(clauseVec);
    }
    std::vector<Var> highLevelVars = Utils::flattenVector<Var>(data.highLevelVars);
    for(int i = 0; i < highLevelVars.size(); i++) {
        vec<Lit> highLevelClause;
        highLevelClause.push(mkLit(highLevelVars[i],false));
        formula->addSoftClause(1, highLevelClause);
    }
}

void TimeTabler::addClauses(Clauses clauses) {
    addClauses(clauses.getClauses());
}

bool TimeTabler::solve() {
    solver->loadFormula(formula);
    model = solver->tSearch();
    if(checkAllTrue(Utils::flattenVector<Var>(data.highLevelVars))) {
        return true;
    } 
    return false;  
}

bool TimeTabler::checkAllTrue(std::vector<Var> inputs) {
    for(int i = 0; i < inputs.size(); i++) {
        if(model[inputs[i]] == l_False) {
            return false;
        }
    }
    return true;
}

bool TimeTabler::isVarTrue(Var v) {
    if (model[v] == l_False) {
        return false;
    }
    return true;
}

Var TimeTabler::newVar() {
    Var var = formula->nVars();
    formula->newVar();
    return var;
}

Lit TimeTabler::newLiteral(bool sign = false) {
    Lit p = mkLit(formula->nVars(), sign);
    formula->newVar();
    return p;
}

void TimeTabler::printResult() {
    if(checkAllTrue(Utils::flattenVector<Var>(data.highLevelVars))) {
        std::cout << "All high level clauses were satisfied" << std::endl;
        displayTimeTable();
    }
    else {
        std::cout << "Some high level clauses were not satisfied" << std::endl;
        displayUnsatisfiedOutputReasons();
    }
}

void TimeTabler::displayTimeTable() {
    for(int i = 0; i < data.courses.size(); i++) {
        std::cout << "Course : " << data.courses[i].getName() << std::endl;
        for(int j = 0; j < data.fieldValueVars[i][FieldType::slot].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::slot][j])) {
                std::cout << "Slot : " << data.slots[j].getName() << std::endl;
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::classroom].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::classroom][j])) {
                std::cout << "Classroom : " << data.classrooms[j].getName() << std::endl;
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::segment].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::segment][j])) {
                std::cout << "Segment : " << data.segments[j].toString() << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void TimeTabler::displayUnsatisfiedOutputReasons() {
    for(int i = 0; i < data.highLevelVars.size(); i++) {
        for(int j = 0; j < data.highLevelVars[i].size(); j++) {
            if(!isVarTrue(data.highLevelVars[i][j])) {
                std::cout << "Field : " << Utils::getFieldTypeName(FieldType(j));
                std::cout << " of Course : " << data.courses[i].getName();
                std::cout << "could not be satisfied" << std::endl;
            }
        }
    }
}