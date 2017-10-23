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

/**
 * @brief      Constructs the TimeTabler object.
 */
TimeTabler::TimeTabler() {
    solver = new TSolver(1, _CARD_TOTALIZER_);
    formula = new MaxSATFormula();
    formula->setProblemType(_WEIGHTED_);
}

/**
 * @brief      Adds clauses to the solver with specified weights.
 * 
 * A negative weight implies that the clauses are had, and a zero weight implies
 * that the clauses are not added to the solver.
 *
 * @param[in]  clauses  The clauses
 * @param[in]  weight   The weight
 */
void TimeTabler::addClauses(const std::vector<CClause> &clauses, int weight) {
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clauseVec;
        std::vector<Lit> clauseVector = clauses[i].getLits();
        for(int j = 0; j < clauseVector.size(); j++) {
            clauseVec.push(clauseVector[j]);
        }
        addToFormula(clauseVec, weight);
    }
}

/**
 * @brief      Adds unit soft clauses for the high level variables to the solver.
 */
void TimeTabler::addHighLevelClauses() {
    for(int i = 0; i < Global::FIELD_COUNT; i++) {
        for(int j = 0; j < data.highLevelVars.size(); j++) {
            vec<Lit> highLevelClause;
            highLevelClause.clear();
            highLevelClause.push(mkLit(data.highLevelVars[j][i],false));
            addToFormula(highLevelClause, data.highLevelVarWeights[i]);
        }
    }
}

/**
 * @brief      Adds unit clauses corresponding to existing assignments given in the input
 *             to the solver.
 */
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

/**
 * @brief      Add a given vec of literals with the given weight to the formula.
 * 
 * A negative weight implies that the clauses are had, and a zero weight implies
 * that the clauses are not added to the solver.
 *
 * @param      input   The input
 * @param[in]  weight  The weight
 */
void TimeTabler::addToFormula(vec<Lit> &input, int weight) {
    if(weight == -1) {
        formula->addHardClause(input);
    }
    else if(weight > 0) {
        formula->addSoftClause(weight, input);
    }
}

/**
 * @brief      Adds clauses to the solver with specified weights.
 * 
 * A negative weight implies that the clauses are had, and a zero weight implies
 * that the clauses are not added to the solver.
 *
 * @param[in]  clauses  The clauses
 * @param[in]  weight   The weight
 */
void TimeTabler::addClauses(const Clauses &clauses, int weight) {
    addClauses(clauses.getClauses(), weight);
}

/**
 * @brief      Calls the solver to solve for the constraints.
 *
 * @return     True, if all high level variables were satisfied, False otherwise
 */
bool TimeTabler::solve() {
    solver->loadFormula(formula);
    model = solver->tSearch();
    if(checkAllTrue(Utils::flattenVector<Var>(data.highLevelVars))) {
        return true;
    } 
    return false;  
}

/**
 * @brief      Checks if a given set of variables are true in the model returned by the solver.
 *
 * @param[in]  inputs  The input variables
 *
 * @return     True, if all variables are True, False otherwise
 */
bool TimeTabler::checkAllTrue(const std::vector<Var> &inputs) {
    for(int i = 0; i < inputs.size(); i++) {
        if(model[inputs[i]] == l_False) {
            return false;
        }
    }
    return true;
}

/**
 * @brief      Determines if a given variable is true in the model returned by the solver.
 *
 * @param[in]  v     The variable to be checked
 *
 * @return     True if variable true, False otherwise
 */
bool TimeTabler::isVarTrue(const Var &v) {
    if (model[v] == l_False) {
        return false;
    }
    return true;
}

/**
 * @brief      Calls the formula to issue a new variable and returns it.
 *
 * @return     The new Var added to the formula
 */
Var TimeTabler::newVar() {
    Var var = formula->nVars();
    formula->newVar();
    return var;
}

/**
 * @brief      Calls the formula to issue a new literal and returns it.
 *
 * @param[in]  sign  The sign, whether the literal contains a sign with the variable
 *
 * @return     The Lit corresponding to the new Var added to the formula
 */
Lit TimeTabler::newLiteral(bool sign = false) {
    Lit p = mkLit(formula->nVars(), sign);
    formula->newVar();
    return p;
}

/**
 * @brief      Prints the result of the problem.
 */
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

/**
 * @brief      Displays the generated time table.
 */
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

/**
 * @brief      Writes the generated time table to a CSV file.
 *
 * @param[in]  fileName  The file path of the output CSV file
 */
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

/**
 * @brief      Displays the reasons due to which the formula could not be
 *             satisfied, if applicable.
 */
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

/**
 * @brief      Destroys the object, and deletes the solver.
 */
TimeTabler::~TimeTabler() {
    delete solver;
}
