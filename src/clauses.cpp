#include "clauses.h"

Clauses::Clauses(std::vector<CClause> clauses) {
	this->clauses = clauses;
}

Clauses::Clauses(CClause clause) {
	clauses.clear();
	clauses.push_back(clause);
}

Clauses::Clauses() {
	clauses.clear();
}

Clauses Clauses::operator~() {
	Clauses result;
	for(int i = 0; i < clauses.size(); i++) {
		std::vector<CClause> negationClause = ~(clauses[i]);
		result.addClauses(negationClause);
	} 
	// INCOMPLETE
}

Clauses Clauses::operator&(const Clauses &other) {
	std::vector<CClause> thisClauses = clauses;
	std::vector<CClause> otherClauses = other.clauses;
	thisClauses.insert(std::end(thisClauses), std::begin(otherClauses), std::end(otherClauses));
	Clauses result(thisClauses);
	return result;
}

Clauses Clauses::operator&(const CClause &other) {
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
	return (other | (*this));
}

Clauses Clauses::operator->(const Clauses &other) {
	return ((~(*this)) | other);
}

void Clauses::addClauses(CClause other) {
	clauses.push_back(other);
}

void Clauses::addClauses(std::vector<CClause> other) {
	clauses.insert(std::end(clauses), std::begin(other), std::end(other));
}

std::vector<CClause> Clauses::getClauses() {
	return clauses;
}