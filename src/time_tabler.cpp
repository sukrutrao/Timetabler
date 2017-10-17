#include "time_tabler.h"

void TimeTabler::addClauses(std::vector<CClause> clauses) {
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clause = Utils::convertVectorToVec<Lit>(clauses.getLits());
        solver->addHardClause(clause);
    }
}

bool TimeTabler::solve() {
    solver->search();
    // search() has side effects. what to do?
}