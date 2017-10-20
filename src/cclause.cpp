#include "cclause.h"

#include <vector>
#include "core/SolverTypes.h"
#include "clauses.h"
#include <iostream>

using namespace Minisat;

CClause::CClause(std::vector<Lit> lits) {
    this->lits = lits;
}

CClause::CClause(Lit lit) {
    lits.clear();
    lits.push_back(lit);
}

CClause::CClause(Var var) {
    lits.clear();
    lits.push_back(mkLit(var, false));
}

CClause::CClause() {
    lits.clear();
}

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

std::vector<CClause> CClause::operator&(CClause &other) {
    std::vector<CClause> result;
    result.clear();
    result.push_back(*this);
    result.push_back(other);
    return result;
}

Clauses CClause::operator&(Clauses &other) {
    Clauses result(other.getClauses());
    result.addClauses(*this);
    return result;
}

CClause CClause::operator|(CClause &other) {
    std::vector<Lit> thisLits = this->lits;
    std::vector<Lit> otherLits = other.getLits();
    thisLits.insert(std::end(thisLits), std::begin(otherLits), std::end(otherLits));
    std::sort(thisLits.begin(), thisLits.end());
    thisLits.erase(std::unique(thisLits.begin(), thisLits.end()), thisLits.end());
    CClause result(thisLits);
    return result;
}

Clauses CClause::operator|(Clauses &other) {
    Clauses thisLHS(*this);
    return (thisLHS | other);
}

/*std::vector<CClause> CClause::operator>>(CClause &other) {
    std::vector<CClause> lhs = ~(*this);
    std::vector<CClause> result;
    result.clear();
    for(int i = 0; i < lhs.size(); i++) {
        CClause thisClause = lhs[i] | other;
        result.push_back(thisClause);
    }
    return result;
}*/

Clauses CClause::operator>>(Clauses &other) {
    Clauses thisLHS(~(*this));
    return (thisLHS | other);
}

void CClause::createLitAndAdd(Var var1) {
    lits.push_back(mkLit(var1, false));
}

void CClause::createLitAndAdd(Var var1, Var var2) {
    lits.push_back(mkLit(var1, false));
    lits.push_back(mkLit(var2, false));
}

void CClause::createLitAndAdd(Var var1, Var var2, Var var3) {
    lits.push_back(mkLit(var1, false));
    lits.push_back(mkLit(var2, false));
    lits.push_back(mkLit(var3, false));
}

void CClause::addLits(Lit lit1) {
    lits.push_back(lit1);
}

void CClause::addLits(Lit lit1, Lit lit2) {
    lits.push_back(lit1);
    lits.push_back(lit2);
}

void CClause::addLits(Lit lit1, Lit lit2, Lit lit3) {
    lits.push_back(lit1);
    lits.push_back(lit2);
    lits.push_back(lit3);
}

void CClause::addLits(std::vector<Lit> otherLits) {
    lits.insert(std::end(lits), std::begin(otherLits), std::end(otherLits));
}

std::vector<Lit> CClause::getLits() {
    return lits;
}

void CClause::printClause() {
    for (Lit lit : lits) {
        std::cout << (sign(lit) ? "-" : " ") << var(lit) << " ";
    }
    std::cout << std::endl;
}

void CClause::clear() {
    lits.clear();
}