class CClause {
public:
    std::vector<Var> vars;
    operator~();
    operator&();
    operator->();
    std::vector<Clause> getCNFClauses();
};

class ConstraintTypes {
public:
    static Clause forAll(<>,Clause);
    static
}
