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
    vec<lbool> model;
public:
    Data data;
    TimeTabler();
    void addClauses(std::vector<CClause>);
    bool solve();
};

#endif