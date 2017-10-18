#include "time_tabler.h"

#include <vector>
#include "cclause.h"
#include "mtl/Vec.h"
#include "core/SolverTypes.h"
#include "tsolver.h"
#include "utils.h"

using namespace Minisat;

TimeTabler::TimeTabler() {
    solver = new TSolver(_VERBOSITY_MINIMAL_, _CARD_TOTALIZER_);
}

void TimeTabler::addClauses(std::vector<CClause> clauses) {
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clause = Utils::convertVectorToVec<Lit>(clauses[i].getLits());
        solver->addHardClause(clause);
    }
}

bool TimeTabler::solve() {
    model = solver->search();
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