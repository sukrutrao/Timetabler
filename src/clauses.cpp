#include "clauses.h"

#include <vector>
#include <iostream>
#include "core/SolverTypes.h"
#include "cclause.h"
#include <iostream>

using namespace Minisat;

/**
 * @brief      Constructs the Clauses object.
 *
 * @param[in]  clauses  The clauses in the set of clauses
 */
Clauses::Clauses(const std::vector<CClause> &clauses) {
    this->clauses = clauses;
}

/**
 * @brief      Constructs the Clauses object.
 *
 * @param[in]  clause  A single clause that forms the set of clauses
 */
Clauses::Clauses(const CClause &clause) {
    clauses.clear();
    clauses.push_back(clause);
}

/**
 * @brief      Constructs the Clauses object.
 *
 * @param[in]  lit   A single literal, a Lit, that is converted
 *                   to a unit clause and forms the set of clauses
 */
Clauses::Clauses(const Lit &lit) {
    clauses.clear();
    CClause clause(lit);
    clauses.push_back(clause);
}

/**
 * @brief      Constructs the Clauses object.
 *
 * @param[in]  var   A single variable, a Var, that is converted
 *                   to a literal with positive polarity, then
 *                   converted to a unit clause, which then forms
 *                   the set of clauses
 */
Clauses::Clauses(const Var &var) {
    clauses.clear();
    CClause clause(var);
    clauses.push_back(clause);
}

/**
 * @brief      Constructs the Clauses object, with no clauses in it
 */
Clauses::Clauses() {
    clauses.clear();
}

/**
 * @brief      Defines the negation operation on a set of clauses.
 * 
 * The negation of a set of clauses ((a1 OR a2) AND (b1 OR b2)) is defined
 * as ((~a1 AND ~a2) OR (~b1 AND ~b2)). The OR operation defined in this
 * class is then used to convert the Clauses to CNF form.
 *
 * @return     The result of the negation operation on the set of clauses
 */
Clauses Clauses::operator~() {
    if(clauses.size() == 0) {
        CClause clause;
        return Clauses(clause);
    }
    Clauses negationClause(~(clauses[0]));
    for(int i = 1; i < clauses.size(); i++) {
        std::vector<CClause> negationClauseVector = ~(clauses[i]);
        Clauses negationThisClause(negationClauseVector);
        negationClause = (negationClause | negationThisClause);
    } 
    return negationClause;
}

/**
 * @brief      Defines the conjunction operation between two sets of clauses.
 * 
 * The conjunction of a set of clauses ((a1 OR a2) AND (b1 OR b2)) with
 * ((x1 OR x2) AND (y1 OR y2)) is given by
 * ((a1 OR a2) AND (b1 OR b2) AND (x1 OR x2) AND (y1 OR y2)). Thus,
 * this function performs this concatenation of clauses and returns
 * a Clauses object containing all the clauses.
 *
 * @param      other  The Clauses with which AND is performed
 *
 * @return     A Clauses object with the result of the AND operation
 */
Clauses Clauses::operator&(const Clauses &other) {
    std::vector<CClause> thisClauses = clauses;
    std::vector<CClause> otherClauses = other.clauses;
    thisClauses.insert(std::end(thisClauses), std::begin(otherClauses), std::end(otherClauses));
    Clauses result(thisClauses);
    return result;
}

/**
 * @brief      Defines the conjunction operation between a Clauses and a CClause.
 *
 * @param      other  The CClause with which the AND operation is performed
 *
 * @return     A Clauses object with the result of the AND operation
 */
Clauses Clauses::operator&(const CClause &other) {
    Clauses otherClauses(other);
    return operator&(otherClauses);
}

/**
 * @brief      Defines the disjunction operation between two sets of clauses.
 * 
 * The disjunction of a set of clauses ((a1 OR a2) AND (b1 OR b2)) with
 * ((x1 OR x2) AND (y1 OR y2)) is given by
 * ((a1 OR a2 OR x1 OR x2) AND (a1 OR a2 OR y1 OR y2) AND (b1 OR b2 OR x1 OR x2)
 * AND (b1 OR b2 OR y1 OR y2)). This function performs this operation and returns
 * a Clauses object with the resultant clauses. Given m clauses in the first operand
 * and n clauses in the second operand, the solution has O(mn) clauses. Using auxiliary
 * variables could have given a O(m+n) equisatisfiable formula. However,
 * as the Clauses object might be further used as an antecedent of an implication, we
 * need that if the resulting clauses of the disjunction are False, the original set
 * of clauses should also have been False, a property which cannot be guaranteed by
 * equisatisfiability alone. Hence, this less efficient method is used for correctness.
 * The resultant Clauses are kept in CNF form.
 *
 * @param      other  The Clauses object to perform the OR operation with
 *
 * @return     A Clauses object with the result of the OR operation
 */
Clauses Clauses::operator|(const Clauses &other) {
    if(other.getClauses().size() == 0) {
        Clauses result = other;
        return result;
    }
    std::vector<CClause> resultClauses;
    resultClauses.clear();
/*    CClause auxiliary;
    Lit newAuxVars[2];
    for(int i = 0; i < 2; i++) {
        newAuxVars[i] = Global::timeTabler->newLiteral(false);
        auxiliary.addLits(newAuxVars[i]);
    }
    resultClauses.push_back(auxiliary);
    for(int i = 0; i < clauses.size(); i++) {
        resultClauses.push_back(CClause(~newAuxVars[0]) | clauses[i]);
    }
    for(int i = 0; i < other.clauses.size(); i++) {
        resultClauses.push_back(CClause(~newAuxVars[1]) | other.clauses[i]);
    }*/
    for(int i = 0; i < clauses.size(); i++) {
        for(int j = 0; j < other.clauses.size(); j++) {
            resultClauses.push_back((clauses[i]) | (other.clauses[j]));
        }
    }
    Clauses result(resultClauses);
    return result;
}

/**
 * @brief      Defines the disjunction operation between a Clauses and a CClause.
 *
 * @param      other  The CClause to perform the OR operation with
 *
 * @return     A Clauses object with the result of the OR operation
 */
Clauses Clauses::operator|(const CClause &other) {
    Clauses otherClauses(other);
    return operator|(otherClauses);
}

/**
 * @brief      Defines the implication operation between two sets of Clauses.
 * 
 * The implication of a set of clauses p to a set of clauses q, which is
 * (p->q), is given by (~p OR q). Thus, this operation is performed using
 * the existing definitions of NOT and OR for sets of clauses.
 *
 * @param      other  The Clauses object which is implied by this object
 *
 * @return     A Clauses object with the result of the implication operation
 */
Clauses Clauses::operator>>(const Clauses &other) {
//    std::cout << "Negating\n";
    Clauses negateThis = operator~();
//    std::cout << "Negated\n";
    // negateThis.print();
    return (negateThis | other);
}

/**
 * @brief      Adds a CClause to the set of clauses.
 *
 * @param[in]  other  The CClause to add
 */
void Clauses::addClauses(const CClause &other) {
    clauses.push_back(other);
}

/**
 * @brief      Adds a vector of CClause to the set of clauses.
 *
 * @param[in]  other  The CClause vector to append
 */
void Clauses::addClauses(const std::vector<CClause> &other) {
    clauses.insert(std::end(clauses), std::begin(other), std::end(other));
}

/**
 * @brief      Adds the clauses of a Clauses object to this object.
 *
 * @param[in]  other  The Clauses object whose clauses are to be added
 */
void Clauses::addClauses(const Clauses &other) {
    std::vector<CClause> otherClauses = other.getClauses();
    addClauses(otherClauses);
}

/**
 * @brief      Gets the clauses in this object.
 *
 * @return     The clauses
 */
std::vector<CClause> Clauses::getClauses() const {
    return clauses;
}

/**
 * @brief      Displays the clauses in this object.
 */
void Clauses::print() {
    std::cout << "Clauses :" << std::endl;
    for (CClause c : clauses) {
        c.printClause();
    }
    std::cout << std::endl;
}

/**
 * @brief      Clears the Clauses object by removing all the clauses.
 */
void Clauses::clear() {
    clauses.clear();
}