#include "clauses.h"

#include <vector>
#include "core/SolverTypes.h"
#include "cclause.h"

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
        return (*this);
    }
    Clauses negationClause(~(clauses[0]));
    for(int i = 1; i < clauses.size(); i++) {
        std::vector<CClause> negationClauseVector = ~(clauses[i]);
        Clauses negationThisClause(negationClauseVector);
        negationClause = (negationClause | negationThisClause);
    } 
    return negationClause;
}

Clauses Clauses::operator&(const Clauses &other) {
    std::vector<CClause> thisClauses = clauses;
    std::vector<CClause> otherClauses = other.clauses;
    thisClauses.insert(std::end(thisClauses), std::begin(otherClauses), std::end(otherClauses));
    Clauses result(thisClauses);
    return result;
}

Clauses Clauses::operator&(CClause &other) {
    return (other & (*this));
}

Clauses Clauses::operator|(const Clauses &other) {
    std::vector<CClause> resultClauses;
    resultClauses.clear();
    for(int i = 0; i < clauses.size(); i++) {
        for(int j = 0; j < other.clauses.size(); j++) {
            resultClauses.push_back((clauses[i]) | (other.clauses[j]));
        }
    }
    Clauses result(resultClauses);
    return result;
}

Clauses Clauses::operator|(const CClause &other) {
    return ((*this) | other);
}

Clauses Clauses::operator>>(const Clauses &other) {
    return ((~(*this)) | other);
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

void Clauses::clear() {
    clauses.clear();
}