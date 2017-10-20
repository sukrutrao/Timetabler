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

class TimeTabler {
private:
    TSolver *solver;
    MaxSATFormula *formula;
    std::vector<Var> assumptions;
    std::vector<lbool> model;
public:
    Data data;
    TimeTabler();
    void addClauses(std::vector<CClause>);
    void addClauses(Clauses);
    void addSoftClauses(std::vector<CClause>);
    void addSoftClauses(Clauses);
    bool checkAllTrue(std::vector<Var>);
    bool isVarTrue(Var);
    bool solve();
    Var newVar();
    Lit newLiteral(bool);
    void printResult();
    void displayTimeTable();
    void displayUnsatisfiedOutputReasons();
    void addHighLevelClauses();
};

#endif