#include "time_tabler.h"

#include <vector>
#include "cclause.h"
#include "mtl/Vec.h"
#include "solver.h"
#include "utils.h"

void TimeTabler::TimeTabler() {
    solver = new Solver();
}

void TimeTabler::addClauses(std::vector<CClause> clauses) {
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clause = Utils::convertVectorToVec<Lit>(clauses.getLits());
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