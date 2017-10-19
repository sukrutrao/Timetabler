#include "parser.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "constraint_adder.h"
#include "mtl/Vec.h"
#include "global.h"
#include <iostream>

TimeTabler *Global::timeTabler = nullptr;

int main(int argc, char const *argv[]) {
    TimeTabler *timeTabler = new TimeTabler();
    Global::timeTabler = timeTabler;
    Parser parser(timeTabler);
    parser.parseFields("config/fields.yml");
    parser.parseInput("config/input.csv");
    parser.addVars();

    ConstraintEncoder encoder(timeTabler);
    ConstraintAdder constraintAdder(&encoder, timeTabler);
    timeTabler->addClauses(constraintAdder.addConstraints());
    bool solved = timeTabler->solve();
    if (solved) {
        std::cout << timeTabler->checkAllTrue(timeTabler->data.highLevelVars[0]) << std::endl;
        std::cout << timeTabler->checkAllTrue(timeTabler->data.highLevelVars[1]) << std::endl;
    }
    timeTabler->printResult();
    return 0;
}