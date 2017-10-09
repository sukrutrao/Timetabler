#include "cclause.h"

CClause::CClause(std::vector<Var> vars) {
	this->vars = vars;
}

CClause::CClause(Var var) {
	vars.clear();
	vars.push_back(var);
}

CClause::CClause() {
	vars.clear();
}

std::vector<CClause> CClause::operator~() {
	std::vector<CClause> result;
	result.clear();
	for(int i = 0; i < vars.size(); i++) {
		CClause unitClause(vars[i]);
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
	std::vector<Var> thisVars = this->vars;
	std::vector<Var> otherVars = other.getVars();
	thisVars.insert(std::end(thisVars), std::begin(otherVars), std::end(otherVars));
	CClause result(thisVars);
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

void CClause::addVars(Var var1) {
	vars.push_back(var1);
}

void CClause::addVars(Var var1, Var var2) {
	vars.push_back(var1);
	vars.push_back(var2);
}

void CClause::addVars(Var var1, Var var2, Var var3) {
	vars.push_back(var1);
	vars.push_back(var2);
	vars.push_back(var3);
}

void CClause::addVars(std::vector<Var> otherVars) {
	vars.insert(std::end(vars), std::begin(otherVars), std::end(otherVars));
}

std::vector<Var> CClause::getVars() {
	return vars;
}