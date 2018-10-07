#include <iostream>
#include "constraint_adder.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "custom_parser.h"
#include "global.h"
#include "global_vars.h"
#include "mtl/Vec.h"
#include "parser.h"

Timetabler *timetabler;

int main(int argc, char const *argv[]) {
  if (argc != 5) {
    std::cout << "Run as " << argv[0]
              << " fields.yml input.csv custom.txt output.csv" << std::endl;
    return 0;
  }
  timetabler = new Timetabler();
  Parser parser(timetabler);
  parser.parseFields(std::string(argv[1]));
  parser.parseInput(std::string(argv[2]));
  if (parser.verify()) {
    std::cout << "Input is valid" << std::endl;
  } else {
    std::cout << "Input is invalid" << std::endl;
    exit(1);
  }
  parser.addVars();
  ConstraintEncoder encoder(timetabler);
  ConstraintAdder constraintAdder(&encoder, timetabler);
  constraintAdder.addConstraints();
  parseCustomConstraints(std::string(argv[3]), &encoder, timetabler);
  timetabler->addHighLevelClauses();
  timetabler->addExistingAssignments();
  SolverStatus solverStatus = timetabler->solve();
  timetabler->printResult(solverStatus);
  if (solverStatus == SolverStatus::Solved) {
    timetabler->writeOutput(std::string(argv[4]));
  }
  delete timetabler;
  return 0;
}
