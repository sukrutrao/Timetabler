#include "parser.h"
#include "encoder.h"
#include "core/Solver.h"

int main(int argc, char const *argv[]) {
    TimeTabler *timeTabler = new TimeTabler();
    Parser parser(timeTabler);
    parser.parseFields("config/fields.yml");
    parser.parseInput("config/input.csv");
    parser.addVars();

    Encoder encoder(timeTabler);
    // TODO Constraint Adder
    timeTabler->solve();
    return 0;
}