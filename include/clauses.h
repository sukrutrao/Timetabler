#ifndef CLAUSES_H
#define CLAUSES_H

#include <vector>
#include "cclause.h"

class Clauses {
private:
	std::vector<CClause> clauses;
public:
	Clauses(std::vector<CClause>);
	Clauses(CClause);
	Clauses();
	Clauses operator~();
	Clauses operator&(const Clauses&);
	Clauses operator&(const CClause&);
	Clauses operator|(const Clauses&);
	Clauses operator|(const CClause&);
	Clauses operator->(const Clauses&);
	void addClauses(CClause);
	void addClauses(std::vector<CClause>);
	std::vector<CClause> getClauses();
};

#endif