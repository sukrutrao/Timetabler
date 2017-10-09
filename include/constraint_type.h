
class ConstraintType {
public:
    static std::pair<Variable, Variable> forAll(std::vector<Variable>, std::function<Clauses(Variable,Variable)>);
    static
};