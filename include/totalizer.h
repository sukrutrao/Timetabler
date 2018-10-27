/*!
 * \author Ruben Martins - ruben@sat.inesc-id.pt
 *
 * @section LICENSE
 *
 * Open-WBO, Copyright (c) 2013-2017, Ruben Martins, Vasco Manquinho, Ines Lynce
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef Totalizer_h
#define Totalizer_h

#include "MaxSATFormula.h"
#include "core/Solver.h"
#include "core/SolverTypes.h"
#include "mtl/Vec.h"

using namespace openwbo;
using namespace NSPACE;

class TTotalizer {

  public:
    TTotalizer(int strategy = _INCREMENTAL_NONE_) {
        blocking = lit_Undef;
        hasEncoding = false;
        joinMode = false;
        current_cardinality_rhs = -1; // -1 corresponds to an unitialized value
        incremental_strategy = strategy;

        n_clauses = 0;
        n_variables = 0;
    }
    ~TTotalizer() {}

    void build(MaxSATFormula *formula, vec<Lit> &lits, int64_t rhs);
    void join(MaxSATFormula *formula, vec<Lit> &lits, int64_t rhs);
    void update(MaxSATFormula *formula, int64_t rhs, vec<Lit> &lits,
                vec<Lit> &assumptions);
    void update(MaxSATFormula *formula, int64_t rhs) {
        vec<Lit> lits;
        vec<Lit> assumptions;
        update(formula, rhs, lits, assumptions);
    }

    bool hasCreatedEncoding() { return hasEncoding; }
    void setIncremental(int incremental) { incremental_strategy = incremental; }
    int getIncremental() { return incremental_strategy; }

    int getNbClauses() { return n_clauses; }
    int getNbVariables() { return n_variables; }
    void resetCounters() {
        n_clauses = 0;
        n_variables = 0;
    }

    vec<Lit> &lits() { return ilits; }
    vec<Lit> &outputs() { return cardinality_outlits; }

    void addUnitClause(MaxSATFormula *formula, Lit a);
    void addBinaryClause(MaxSATFormula *formula, Lit a, Lit b);
    void addTernaryClause(MaxSATFormula *formula, Lit a, Lit b, Lit c);

    void newSATVariable(MaxSATFormula *formula);

  protected:
    void encode(MaxSATFormula *formula, vec<Lit> &lits);
    void adder(MaxSATFormula *formula, vec<Lit> &left, vec<Lit> &right,
               vec<Lit> &output);
    void incremental(MaxSATFormula *formula, int64_t rhs);
    void toCNF(MaxSATFormula *formula, vec<Lit> &lits);

    vec<vec<Lit>> totalizerIterative_left;
    vec<vec<Lit>> totalizerIterative_right;
    vec<vec<Lit>> totalizerIterative_output;
    vec<int64_t> totalizerIterative_rhs;

    Lit blocking; // Controls the blocking literal for the incremental blocking.
    bool hasEncoding;

    // TEMP
    vec<Lit> ilits;
    vec<Lit> olits;

    vec<Lit>
        cardinality_inlits; // Stores the inputs of the cardinality
                            // constraint encoding for the totalizer encoding
    vec<Lit> cardinality_outlits; // Stores the outputs of the cardinality
                                  // constraint encoding for incremental solving

    int incremental_strategy;
    int64_t current_cardinality_rhs;

    vec<Lit>
        disable_lits; // Contains a vector with a list of blocking literals.
    bool joinMode;

    int n_clauses;
    int n_variables;
};

#endif
