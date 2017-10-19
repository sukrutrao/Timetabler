#ifndef CLAUSES_H
#define CLAUSES_H

#include <vector>
#include "cclause.h"
#include "time_tabler.h"

using namespace Minisat;

class CClause;

// TODO - handle empty inputs for operators

class Clauses {
private:
    std::vector<CClause> clauses;
public:
    Clauses(std::vector<CClause>);
    Clauses(CClause);
    Clauses(Lit);
    Clauses(Var);
    Clauses();
    Clauses operator~();
    Clauses operator&(Clauses&);
    Clauses operator&(CClause&);
    Clauses operator|(Clauses&);
    Clauses operator|(CClause&);
    Clauses operator>>(Clauses&);
    void addClauses(CClause);
    void addClauses(std::vector<CClause>);
    void addClauses(Clauses);
    std::vector<CClause> getClauses();
    void clear();
};

#endif