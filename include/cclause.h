/** @file */

#ifndef CCLAUSE_H
#define CCLAUSE_H

#include "core/SolverTypes.h"
#include <vector>
class Clauses;

using namespace NSPACE;

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
    CClause(const std::vector<Lit> &);
    CClause(const Lit &);
    CClause(const Var &);
    CClause();
    std::vector<CClause> operator~();
    std::vector<CClause> operator&(const CClause &);
    Clauses operator&(const Clauses &);
    CClause operator|(const CClause &);
    Clauses operator|(const Clauses &);
    std::vector<CClause> operator>>(const CClause &);
    Clauses operator>>(const Clauses &);
    void createLitAndAdd(const Var &);
    void createLitAndAdd(const Var &, const Var &);
    void createLitAndAdd(const Var &, const Var &, const Var &);
    void addLits(const Lit &);
    void addLits(const Lit &, const Lit &);
    void addLits(const Lit &, const Lit &, const Lit &);
    void addLits(const std::vector<Lit> &);
    std::vector<Lit> getLits() const;
    void clear();
    void printClause();
};

#endif
