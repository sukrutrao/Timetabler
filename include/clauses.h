/** @file */

#ifndef CLAUSES_H
#define CLAUSES_H

#include <vector>
#include "cclause.h"
#include "time_tabler.h"

using namespace Minisat;

class CClause;

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
    Clauses(const std::vector<CClause>&);
    Clauses(const CClause&);
    Clauses(const Lit&);
    Clauses(const Var&);
    Clauses();
    Clauses operator~();
    Clauses operator&(const Clauses&);
    Clauses operator&(const CClause&);
    Clauses operator|(const Clauses&);
    Clauses operator|(const CClause&);
    Clauses operator>>(const Clauses&);
    void addClauses(const CClause&);
    void addClauses(const std::vector<CClause>&);
    void addClauses(const Clauses&);
    std::vector<CClause> getClauses() const;
    void print();
    void clear();
};

#endif