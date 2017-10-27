#include "parser.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "constraint_adder.h"
#include "mtl/Vec.h"
#include "global.h"
#include "custom_parser.h"
#include <iostream>

int main(int argc, char const *argv[]) {
    if (argc != 5) {
        std::cout << "Run as " << argv[0] << " fields.yml input.csv custom.txt output.csv" << std::endl;
        return 0;
    }
    TimeTabler *timeTabler = new TimeTabler();
    Parser parser(timeTabler);
    parser.parseFields(std::string(argv[1]));
    parser.parseInput(std::string(argv[2]));
    parser.addVars();
    ConstraintEncoder encoder(timeTabler);
    ConstraintAdder constraintAdder(&encoder, timeTabler);
    constraintAdder.addConstraints();
    parseCustomConstraints(std::string(argv[3]), &encoder, timeTabler);
    timeTabler->addHighLevelClauses();
    timeTabler->addExistingAssignments();
    SolverStatus solverStatus = timeTabler->solve();
    timeTabler->printResult(solverStatus);
    if (solverStatus == SolverStatus::Solved) {
        timeTabler->writeOutput(std::string(argv[4]));
    }
    delete timeTabler;
    return 0;
}
