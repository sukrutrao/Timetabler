#ifndef TIME_TABLER_H
#define TIME_TABLER_H

#include <vector>
#include "core/SolverTypes.h"
#include "core/MaxSAT.h"
#include "data.h"

class TimeTabler {
private:
    Solver *solver;
    std::vector<Var> assumptions;
public:
    Data data;
    TimeTabler();
    void addClauses(std::vector<CClause>);
    bool solve();
};

#endif