class CClause {
private:
	std::vector<Lit> lits;
public:
	CClause(std::vector<Lit>);
	CClause(Lit);
	CClause();
    std::vector<CClause> operator~();
    std::vector<CClause> operator&(const CClause&);
    Clauses operator&(const Clauses&);
    CClause operator|(const CClause&);
    Clauses operator|(const Clauses&);
    std::vector<CClause> operator->(const CClause&);
    Clauses operator->(const Clauses&);
    void addLits(Lit);
    void addLits(Lit,Lit);
    void addLits(Lit,Lit,Lit);
    std::vector<Lit> getLits();
};

class ConstraintType {
public:
    static std::vector<CClause> forAll(Litiable,Litiable,std::vector<CClause>);
    static
};

class FunctionType {
public:
	static 
};
