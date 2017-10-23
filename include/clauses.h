#ifndef CLAUSES_H
#define CLAUSES_H

#include <vector>
#include "cclause.h"
#include "time_tabler.h"

using namespace Minisat;

class CClause;

// TODO - handle empty inputs for operators

/**
 * @brief      Class for representing a set of clauses.
 * 
 * A set of clauses is represented as a vector of type CClause.
 * This class defines operations between sets of clauses, such as 
 * AND, OR, NOT, and IMPLIES. This also defines functions to
 * create Clauses, add clauses, and work with them. All clauses
 * are always maintained in the CNF form.
 */
class Clauses {
private:
    /**
     * The clauses, each a CClause member object, in this set of Clauses
     */
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
    void print();
    void clear();
};

#endif