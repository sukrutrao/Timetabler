#include "cclause.h"

#include <vector>
#include "core/SolverTypes.h"
#include "clauses.h"

CClause::CClause(std::vector<Lit> lits) {
	this->lits = lits;
}

CClause::CClause(Lit lit) {
	lits.clear();
	lits.push_back(lit);
}

CClause::CClause() {
	lits.clear();
}

std::vector<CClause> CClause::operator~() {
	std::vector<CClause> result;
	result.clear();
	for(int i = 0; i < lits.size(); i++) {
		CClause unitClause(~(lits[i]));
		result.push_back(unitClause);
	}
	return result;
}

std::vector<CClause> CClause::operator&(const CClause &other) {
	std::vector<CClause> result;
	result.clear();
	result.push_back(*this);
	result.push_back(other);
	return result;
}

Clauses CClause::operator&(const Clause &other) {
	Clause result(other.getClauses());
	result.addClauses(*this);
	return result;
}

CClause CClause::operator|(const CClause &other) {
	std::vector<Lit> thisLits = this->lits;
	std::vector<Lit> otherLits = other.getLits();
	thisLits.insert(std::end(thisLits), std::begin(otherLits), std::end(otherLits));
	CClause result(thisLits);
	return result;
}

Clauses CClause::operator|(const Clauses &other) {
	Clauses thisLHS(*this);
	return (thisLHS | other);
}

std::vector<CClause> CClause::operator->(const CClause &other) {
	std::vector<CClause> lhs = ~(*this);
	std::vector<CClause> result;
	result.clear();
	for(int i = 0; i < lhs.size(); i++) {
		result.push_back((lhs[i])|other);
	}
	return result;
}

Clauses CClause::operator->(const Clauses &other) {
	Clauses thisLHS(~(*this));
	return (thisLHS | other);
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