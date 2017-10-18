#include "parser.h"
#include "encoder.h"
#include "core/Solver.h"
#include "constraint_adder.h"
#include "mtl/Vec.h"

int main(int argc, char const *argv[]) {
    TimeTabler *timeTabler = new TimeTabler();
    Parser parser(timeTabler);
    parser.parseFields("config/fields.yml");
    parser.parseInput("config/input.csv");
    parser.addVars();

    Encoder encoder(timeTabler);
    ConstraintAdder constraintAdder(encoder, timeTabler);
    ConstraintAdder.addConstraints();
    
    timeTabler->solve();
    return 0;
}