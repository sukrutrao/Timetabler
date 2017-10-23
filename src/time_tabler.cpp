#include "time_tabler.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "cclause.h"
#include "mtl/Vec.h"
#include "core/SolverTypes.h"
#include "tsolver.h"
#include "utils.h"
#include "MaxSATFormula.h"
#include "clauses.h"

using namespace Minisat;

TimeTabler::TimeTabler() {
    solver = new TSolver(1, _CARD_TOTALIZER_);
    formula = new MaxSATFormula();
    formula->setProblemType(_WEIGHTED_);
}

void TimeTabler::addClauses(const std::vector<CClause> &clauses, int weight) {
    std::cout << "Clause count : " << clauses.size() << " W : " << weight << std::endl;
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clauseVec;
        std::vector<Lit> clauseVector = clauses[i].getLits();
        for(int j = 0; j < clauseVector.size(); j++) {
            // if(sign(clauseVector[j])) { std::cout << "-"; }
            // std::cout << var(clauseVector[j]) << " ";
            clauseVec.push(clauseVector[j]);
        }
        // std::cout << std::endl;
        addToFormula(clauseVec, weight);

    }
}

void TimeTabler::addHighLevelClauses() {
  /*  for(int i = 0; i < data.courses.size(); i++) {
        for(int j = 0; j < 6; j++) {
            // std::cout << "DHL : " << data.highLevelVars[i][j] << std::endl;
        }
    }*/
   /* std::vector<Var> highLevelVars = Utils::flattenVector<Var>(data.highLevelVars);
    for(int i = 0; i < highLevelVars.size(); i++) {
        // std::cout << "DHLF : " << highLevelVars[i] << std::endl;
    }
    for(int i = 0; i < highLevelVars.size(); i++) {
        vec<Lit> highLevelClause;
        highLevelClause.clear();
        highLevelClause.push(mkLit(highLevelVars[i],false));
        formula->addSoftClause(10, highLevelClause);
    }*/
    for(int i = 0; i < Global::FIELD_COUNT; i++) {
        for(int j = 0; j < data.highLevelVars.size(); j++) {
            vec<Lit> highLevelClause;
            highLevelClause.clear();
            highLevelClause.push(mkLit(data.highLevelVars[j][i],false));
            addToFormula(highLevelClause, data.highLevelVarWeights[i]);
        }
    }
}

void TimeTabler::addExistingAssignments() {
    for(int i = 0; i < data.existingAssignmentVars.size(); i++) {
        for(int j = 0; j < data.existingAssignmentVars[i].size(); j++) {
            for(int k = 0; k < data.existingAssignmentVars[i][j].size(); k++) {
                if(data.existingAssignmentVars[i][j][k] == l_Undef) {
                    continue;
                }
                vec<Lit> clause;
                clause.clear();
                if(data.existingAssignmentVars[i][j][k] == l_True) {
                    clause.push(mkLit(data.fieldValueVars[i][j][k]));
                }
                else {
                    clause.push(~mkLit(data.fieldValueVars[i][j][k]));    
                }
                addToFormula(clause, data.existingAssignmentWeights[j]);
            }
        }
    }
}

void TimeTabler::addToFormula(vec<Lit> &input, int weight) {
    if(weight < 0) {
        formula->addHardClause(input);
    }
    else if(weight > 0) {
        formula->addSoftClause(weight, input);
    }
}

void TimeTabler::addClauses(const Clauses &clauses, int weight) {
    addClauses(clauses.getClauses(), weight);
}

/*void TimeTabler::addSoftClauses(std::vector<CClause> clauses) {
    std::cout << "Soft Clause count : " << clauses.size() << std::endl;
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clauseVec;
        std::vector<Lit> clauseVector = clauses[i].getLits();
        for(int j = 0; j < clauseVector.size(); j++) {
            clauseVec.push(clauseVector[j]);
        }
        formula->addSoftClause(1, clauseVec);
    }    
}

void TimeTabler::addSoftClauses(const Clauses &clauses) {
    addSoftClauses(clauses.getClauses());
}*/

bool TimeTabler::solve() {
    solver->loadFormula(formula);
    if(formula->getProblemType() == _WEIGHTED_) {
        std::cout << "WEIGHTED" << std::endl;
    }
    // solver->search();
    model = solver->tSearch();
    if(checkAllTrue(Utils::flattenVector<Var>(data.highLevelVars))) {
        return true;
    } 
    return false;  
}

bool TimeTabler::checkAllTrue(const std::vector<Var> &inputs) {
    for(int i = 0; i < inputs.size(); i++) {
        if (model.size() == 0) return false;
        if(model[inputs[i]] == l_False) {
            return false;
        }
    }
    return true;
}

bool TimeTabler::isVarTrue(const Var &v) {
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
                std::cout << "Slot : " << data.slots[j].getName() << " " << data.fieldValueVars[i][FieldType::slot][j] << std::endl;
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::instructor].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::instructor][j])) {
                std::cout << "Instructor : " << data.instructors[j].getName() << " " << data.fieldValueVars[i][FieldType::instructor][j] << std::endl;
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::classroom].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::classroom][j])) {
                std::cout << "Classroom : " << data.classrooms[j].getName() << " " << data.fieldValueVars[i][FieldType::classroom][j] << std::endl;
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::segment].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::segment][j])) {
                std::cout << "Segment : " << data.segments[j].getName() << " " << data.fieldValueVars[i][FieldType::segment][j] << std::endl;
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::program].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::program][j])) {
                std::cout << "Program : " << data.programs[j/2].getName() << " ";
                if(j%2 == 0) {
                    std::cout << "Core" << std::endl;
                }
                else {
                    std::cout << "Elective" << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }
}

void TimeTabler::writeOutput(std::string fileName) {
    std::ofstream fileObject;
    fileObject.open(fileName);
    fileObject << "name,class_size,instructor,segment,is_minor,";
    for(int i = 0; i < data.programs.size(); i+=2) {
        fileObject << data.programs[i].getName() << ",";
    }
    fileObject << "classroom,slot" << std::endl;
    for(int i = 0; i < data.courses.size(); i++) {
        fileObject << data.courses[i].getName() << "," << data.courses[i].getClassSize() << ",";
        for(int j = 0; j < data.fieldValueVars[i][FieldType::instructor].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::instructor][j])) {
                fileObject << data.instructors[j].getName() << ",";
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::segment].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::segment][j])) {
                fileObject << data.segments[j].getName() << ",";
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::isMinor].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::isMinor][j])) {
                fileObject << data.isMinors[j].getName() << ",";
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::program].size(); j+=2) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::program][j])) {
                fileObject << data.programs[j].getCourseTypeName() << ",";
            }
            else if(isVarTrue(data.fieldValueVars[i][FieldType::program][j+1])) {
                fileObject << data.programs[j+1].getCourseTypeName() << ",";
            }
            else {
                fileObject << "No,";
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::classroom].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::classroom][j])) {
                fileObject << data.classrooms[j].getName() << ",";
            }
        }
        for(int j = 0; j < data.fieldValueVars[i][FieldType::slot].size(); j++) {
            if(isVarTrue(data.fieldValueVars[i][FieldType::slot][j])) {
                fileObject << data.slots[j].getName();
            }
        }
        fileObject << std::endl;
    }
    fileObject.close();
}

void TimeTabler::displayUnsatisfiedOutputReasons() {
    for(int i = 0; i < data.highLevelVars.size(); i++) {
        for(int j = 0; j < data.highLevelVars[i].size(); j++) {
            if(!isVarTrue(data.highLevelVars[i][j])) {
                std::cout << "Field : " << Utils::getFieldTypeName(FieldType(j));
                std::cout << " of Course : " << data.courses[i].getName();
                std::cout << " could not be satisfied" << std::endl;
            }
        }
    }
}

TimeTabler::~TimeTabler() {
    delete solver;
}
