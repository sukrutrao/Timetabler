#include "time_tabler.h"

#include <vector>
#include "cclause.h"
#include "mtl/Vec.h"
#include "core/SolverTypes.h"
#include "tsolver.h"
#include "utils.h"
#include "MaxSATFormula.h"
#include "clauses.h"

using namespace Minisat;

TimeTabler::TimeTabler() {
    solver = new TSolver(_VERBOSITY_MINIMAL_, _CARD_TOTALIZER_);
    formula = new MaxSATFormula();
}

void TimeTabler::addClauses(std::vector<CClause> clauses) {
    vec<vec<Lit>> clausesVec;
    for(int i = 0; i < clauses.size(); i++) {
        vec<Lit> clauseVec;
        std::vector<Lit> clauseVector = clauses[i].getLits();
        for(int j = 0; j < clauseVector.size(); j++) {
            clauseVec.push(clauseVector[j]);
        }
        formula->addHardClause(clauseVec);
    }
    std::vector<Var> highLevelVars = Utils::flattenVector<Var>(data.highLevelVars);
    for(int i = 0; i < highLevelVars.size(); i++) {
        vec<Lit> highLevelClause;
        highLevelClause.push(mkLit(highLevelVars[i],false));
        formula->addSoftClause(1, highLevelClause);
    }
}

void TimeTabler::addClauses(Clauses clauses) {
    addClauses(clauses.getClauses());
}

bool TimeTabler::solve() {
    solver->loadFormula(formula);
    model = solver->tSearch();
    if(checkAllTrue(Utils::flattenVector<Var>(data.highLevelVars))) {
        return true;
    } 
    return false;  
}

bool TimeTabler::checkAllTrue(std::vector<Var> inputs) {
    for(int i = 0; i < inputs.size(); i++) {
        if(model[inputs[i]] == l_False) {
            return false;
        }
    }
    return true;
}

bool TimeTabler::checkVar(Var v) {
    if (model[v] == l_False) {
        return false;
    }
    return true;
}

Var TimeTabler::newVar() {
    Var var = formula->nVars();
    formula->newVar();
    return var;
}

Lit TimeTabler::newLiteral(bool sign = false) {
    Lit p = mkLit(formula->nVars(), sign);
    formula->newVar();
    return p;
}