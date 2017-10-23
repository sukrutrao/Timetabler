#include "cclause.h"

#include <vector>
#include "core/SolverTypes.h"
#include "clauses.h"
#include <iostream>

using namespace Minisat;

/**
 * @brief      Constructs the CClause object.
 *
 * @param[in]  lits  The literals in the clause
 */
CClause::CClause(const std::vector<Lit> &lits) {
    this->lits = lits;
}

/**
 * @brief      Constructs the CClause object.
 *
 * @param[in]  lit   A literal, which creates a unit clause
 */
CClause::CClause(const Lit &lit) {
    lits.clear();
    lits.push_back(lit);
}

/**
 * @brief      Constructs the CClause object.
 *
 * @param[in]  var   A variable, which is converted to a Lit with
 *                   positive polarity and added to create a unit clause
 */
CClause::CClause(const Var &var) {
    lits.clear();
    lits.push_back(mkLit(var, false));
}

/**
 * @brief      Constructs the CClause object, as an empty clause.
 */
CClause::CClause() {
    lits.clear();
}

/**
 * @brief      Defines the operator to negate the clause in this object.
 * 
 * The negation of a clause (a1 OR a2 OR a3) is given by
 * (~a1 AND ~a2 AND ~a3). Thus, the result is a set of clauses,
 * and hence, this function returns this as a vector of CClause.
 *
 * @return     The negation of the clause
 */
std::vector<CClause> CClause::operator~() {
    std::vector<Lit> litCopy = lits;
    std::sort(litCopy.begin(), litCopy.end());
    litCopy.erase(std::unique(litCopy.begin(), litCopy.end()), litCopy.end());
    std::vector<CClause> result;
    result.clear();
    for(int i = 0; i < litCopy.size(); i++) {
        CClause unitClause(~(litCopy[i]));
        result.push_back(unitClause);
    }
    return result;
}

/**
 * @brief      Defines the operator to perform conjunction of two clauses.
 * 
 * The conjunction of clause (a1 OR a2) and (b1 OR b2) is given by
 * ((a1 OR a2) AND (b1 OR b2)). This function simply returns a vector with
 * both clauses appended to it.
 *
 * @param      other  The clause to conjunct with
 *
 * @return     The vector with the clauses as the result of performing
 *             the AND operation 
 */
std::vector<CClause> CClause::operator&(const CClause &other) {
    std::vector<CClause> result;
    result.clear();
    result.push_back(*this);
    result.push_back(other);
    return result;
}

/**
 * @brief      Defines the operator to perform the conjunction of a clause with
 *             a set of clauses.
 *             
 * Given a clause and a set of clauses, the conjunction is simply appending all the
 * clauses together. This does that and returns a Clauses object with all the clauses.
 *
 * @param      other  The set of clauses to conjunct with.
 *
 * @return     A Clauses object with the result of performing the AND operation
 */
Clauses CClause::operator&(const Clauses &other) {
    Clauses result(other.getClauses());
    result.addClauses(*this);
    return result;
}

/**
 * @brief      Defines the operator to perform a disjunction of two clauses.
 * 
 * The disjunction of clause (a1 OR a2) and (b1 OR b2) is given by
 * (a1 OR a2 OR b1 OR b2). This is achieved by appending the literals of the
 * two clauses and returning the resultant clause. Extra checks are added to avoid
 * repeating literals.
 *
 * @param      other  The clause to disjunct with
 *
 * @return     The clause with the result on performing the OR operation
 */
CClause CClause::operator|(const CClause &other) {
    std::vector<Lit> thisLits = this->lits;
    std::vector<Lit> otherLits = other.getLits();
    thisLits.insert(std::end(thisLits), std::begin(otherLits), std::end(otherLits));
    std::sort(thisLits.begin(), thisLits.end());
    thisLits.erase(std::unique(thisLits.begin(), thisLits.end()), thisLits.end());
    bool existBothPolarities = false;
    int indexLitBothPolarities = -1;
    for(int i = 0; i < thisLits.size(); i++) {
        for(int j = i+1; j < thisLits.size(); j++) {
            if(var(thisLits[i]) == var(thisLits[j])) {
                existBothPolarities = true;
                indexLitBothPolarities = i;
                break;
            }
        }
        if(existBothPolarities) {
            break;
        }
    }
    if(existBothPolarities) {
        CClause result;
        result.addLits(thisLits[indexLitBothPolarities]);
        result.addLits(~thisLits[indexLitBothPolarities]);
        return result;
    }
    CClause result(thisLits);
    return result;
}

/**
 * @brief      Defines the operator to perform a disjunction between a clause and a set of
 *             Clauses.
 *             
 * The working is described in the operator defined for OR between two Clauses objects. This
 * function converts the current clause to a Clauses object and performs the operation.
 *
 * @param      other  The Clauses to disjunct with
 *
 * @return     A Clauses with the result of performing the AND operation
 */
Clauses CClause::operator|(const Clauses &other) {
    Clauses thisLHS(*this);
    return (thisLHS | other);
}


/**
 * @brief      Defines the operator to perform an implication with another clause.
 * 
 * Given this clause (a1 OR a2) and another clause (b1 OR b2), this defines
 * ((a1 OR a2) -> (b1 OR b2)). This is done using the fact that (p->q) is 
 * equivalent to (~p OR q). This clause acts as the antecedent, and the
 * argument as the consequent. The result is converted to CNF form and returned.
 *
 * @param      other  The clause which is the consequent
 *
 * @return     A vector of clauses as a result of the implication
 */
std::vector<CClause> CClause::operator>>(const CClause &other) {
    std::cout << "#################################################################\n";
    std::vector<CClause> lhs = ~(*this);
    std::vector<CClause> result;
    result.clear();
    for(int i = 0; i < lhs.size(); i++) {
        CClause thisClause = lhs[i] | other;
        result.push_back(thisClause);
    }
    return result;
}

/**
 * @brief      Defines the operator to perform an implication with a set of Clauses.
 * 
 * Given this clause p and a set of Clauses q, this computes and returns (p->q) in
 * CNF form.
 *
 * @param      other  The Clauses which form the consequent
 *
 * @return     A Clauses object as a result of the implication
 */
Clauses CClause::operator>>(const Clauses &other) {
    Clauses thisLHS(~(*this));
    return (thisLHS | other);
}

/**
 * @brief      Creates a Lit, a literal, given a variable, a Var, and adds it to the clause.
 * 
 * The literal is created with positive polarity.
 *
 * @param[in]  var1  The variable to be added
 */
void CClause::createLitAndAdd(const Var &var1) {
    lits.push_back(mkLit(var1, false));
}

/**
 * @brief      Creates Lits given two Vars and adds them to the clause.
 * 
 * The literals are created with positive polarity.
 *
 * @param[in]  var1  The first Var
 * @param[in]  var2  The second Var
 */
void CClause::createLitAndAdd(const Var &var1, const Var &var2) {
    lits.push_back(mkLit(var1, false));
    lits.push_back(mkLit(var2, false));
}

/**
 * @brief      Creates Lits given three Vars and adds them to the clause.
 * 
 * The literals are created with positive polarity.
 *
 * @param[in]  var1  The first Var
 * @param[in]  var2  The second Var
 * @param[in]  var3  The third Var
 */
void CClause::createLitAndAdd(const Var &var1, const Var &var2, const Var &var3) {
    lits.push_back(mkLit(var1, false));
    lits.push_back(mkLit(var2, false));
    lits.push_back(mkLit(var3, false));
}

/**
 * @brief      Adds a Lit, a literal, to the clause.
 *
 * @param[in]  lit1  The literal to be added
 */
void CClause::addLits(const Lit &lit1) {
    lits.push_back(lit1);
}

/**
 * @brief      Adds two Lits to the clause.
 *
 * @param[in]  lit1  The first Lit
 * @param[in]  lit2  The second Lit
 */
void CClause::addLits(const Lit &lit1, const Lit &lit2) {
    lits.push_back(lit1);
    lits.push_back(lit2);
}

/**
 * @brief      Adds three Lits to the clause.
 *
 * @param[in]  lit1  The first Lit
 * @param[in]  lit2  The second Lit
 * @param[in]  lit3  The third Lit
 */
void CClause::addLits(const Lit &lit1, const Lit &lit2, const Lit &lit3) {
    lits.push_back(lit1);
    lits.push_back(lit2);
    lits.push_back(lit3);
}

/**
 * @brief      Adds a vector of Lits to the clause.
 *
 * @param[in]  otherLits  The vector of Lits to be added
 */
void CClause::addLits(const std::vector<Lit> &otherLits) {
    lits.insert(std::end(lits), std::begin(otherLits), std::end(otherLits));
}

/**
 * @brief      Gets the literals in the clause.
 *
 * @return     The literals in the clause
 */
std::vector<Lit> CClause::getLits() const {
    return lits;
}

/**
 * @brief      Displays the clause.
 */
void CClause::printClause() {
    for (Lit lit : lits) {
        std::cout << (sign(lit) ? "-" : " ") << var(lit) << " ";
    }
    std::cout << std::endl;
}

/**
 * @brief      Clears the clause by removing all the literals.
 */
void CClause::clear() {
    lits.clear();
}