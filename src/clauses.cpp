#include "clauses.h"

#include <vector>
#include <iostream>
#include "core/SolverTypes.h"
#include "cclause.h"
#include <iostream>

using namespace Minisat;

Clauses::Clauses(std::vector<CClause> clauses) {
    this->clauses = clauses;
}

Clauses::Clauses(CClause clause) {
    clauses.clear();
    clauses.push_back(clause);
}

Clauses::Clauses(Lit lit) {
    clauses.clear();
    CClause clause(lit);
    clauses.push_back(clause);
}

Clauses::Clauses(Var var) {
    clauses.clear();
    CClause clause(var);
    clauses.push_back(clause);
}

Clauses::Clauses() {
    clauses.clear();
}

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

Clauses Clauses::operator&(Clauses &other) {
    std::vector<CClause> thisClauses = clauses;
    std::vector<CClause> otherClauses = other.clauses;
    thisClauses.insert(std::end(thisClauses), std::begin(otherClauses), std::end(otherClauses));
    Clauses result(thisClauses);
    return result;
}

Clauses Clauses::operator&(CClause &other) {
    Clauses otherClauses(other);
    return operator&(otherClauses);
}

Clauses Clauses::operator|(Clauses &other) {
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

Clauses Clauses::operator|(CClause &other) {
    Clauses otherClauses(other);
    return operator|(otherClauses);
}

Clauses Clauses::operator>>(Clauses &other) {
    std::cout << "Negating\n";
    Clauses negateThis = operator~();
    std::cout << "Negated\n";
    // negateThis.print();
    return (negateThis | other);
}

void Clauses::addClauses(CClause other) {
    clauses.push_back(other);
}

void Clauses::addClauses(std::vector<CClause> other) {
    clauses.insert(std::end(clauses), std::begin(other), std::end(other));
}

void Clauses::addClauses(Clauses other) {
    std::vector<CClause> otherClauses = other.getClauses();
    addClauses(otherClauses);
}

std::vector<CClause> Clauses::getClauses() {
    return clauses;
}

void Clauses::print() {
    std::cout << "Clauses :" << std::endl;
    for (CClause c : clauses) {
        c.printClause();
    }
    std::cout << std::endl;
}

void Clauses::clear() {
    clauses.clear();
}