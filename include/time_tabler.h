#ifndef TIME_TABLER_H
#define TIME_TABLER_H

#include <vector>
#include "core/SolverTypes.h"
#include "mtl/Vec.h"
#include "cclause.h"
#include "data.h"
#include "tsolver.h"
#include "MaxSATFormula.h"

using namespace Minisat;

/**
 * @brief      Class for time tabler.
 * 
 * This class is responsible for handling the solver.
 * This accepts constraints, adds it to the MaxSATFormula,
 * calls the solver, and interprets and displays the result.
 * It is also responsible for writing the output to a CSV,
 * and creating new literals or variables in the solver when
 * requested and returning them.
 */
class TimeTabler {
private:
    /**
     * A pointer to the MaxSAT solver object
     */
    TSolver *solver;
    /**
     * A pointer to the MaxSAT formula object
     */
    MaxSATFormula *formula;
    /**
     * Stores the values of each solver variable to be checked after solving
     */
    std::vector<lbool> model;
public:
    Data data;
    TimeTabler();
    ~TimeTabler();
    void addClauses(const std::vector<CClause>&, int);
    void addClauses(const Clauses&, int);
    bool checkAllTrue(const std::vector<Var>&);
    bool isVarTrue(const Var&);
    bool solve();
    Var newVar();
    Lit newLiteral(bool);
    void printResult();
    void displayTimeTable();
    void displayUnsatisfiedOutputReasons();
    void addHighLevelClauses();
    void writeOutput(std::string);
    void addExistingAssignments();
    void addToFormula(vec<Lit>&, int);
};

#endif