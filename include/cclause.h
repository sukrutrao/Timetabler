#ifndef CCLAUSE_H
#define CCLAUSE_H

#include <vector>
#include "core/SolverTypes.h"
class Clauses;

using namespace Minisat;

// TODO - handle empty inputs for operators

/**
 * @brief      Class for  representing a clause.
 * 
 * A clause is represented as a vector of Lits. This class
 * defines operators for operations between clauses, such as AND, 
 * OR, NOT, and IMPLIES, and functions to add literals and work 
 * with them in the clause. After each operation,
 * the clause is maintained in the CNF form.
 */
class CClause {
private:
    /**
     * The literals in the clause.
     */
    std::vector<Lit> lits;
public:
    CClause(std::vector<Lit>);
    CClause(Lit);
    CClause(Var);
    CClause();
    std::vector<CClause> operator~();
    std::vector<CClause> operator&(CClause&);
    Clauses operator&(Clauses&);
    CClause operator|(CClause&);
    Clauses operator|(Clauses&);
    std::vector<CClause> operator>>(CClause&);
    Clauses operator>>(Clauses&);
    void createLitAndAdd(Var);
    void createLitAndAdd(Var,Var);
    void createLitAndAdd(Var,Var,Var);
    void addLits(Lit);
    void addLits(Lit,Lit);
    void addLits(Lit,Lit,Lit);
    void addLits(std::vector<Lit>);
    std::vector<Lit> getLits();
    void clear();
    void printClause();
};

#endif