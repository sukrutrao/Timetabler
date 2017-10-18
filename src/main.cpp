#include "parser.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "constraint_adder.h"
#include "mtl/Vec.h"

int main(int argc, char const *argv[]) {
    TimeTabler *timeTabler = new TimeTabler();
    Parser parser(timeTabler);
    parser.parseFields("config/fields.yml");
    parser.parseInput("config/input.csv");
    parser.addVars();

    ConstraintEncoder encoder(timeTabler);
    ConstraintAdder constraintAdder(&encoder, timeTabler);
    timeTabler->addClauses(constraintAdder.addConstraints());
    timeTabler->solve();
    return 0;
}