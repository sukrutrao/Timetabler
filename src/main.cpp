#include "constraint_adder.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "custom_parser.h"
#include "global.h"
#include "global_vars.h"
#include "glog/logging.h"
#include "mtl/Vec.h"
#include "parser.h"
#include <iostream>

Timetabler *timetabler;

int main(int argc, char const *argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = 1;
  FLAGS_colorlogtostderr = 1;
  if (argc != 5) {
    LOG(ERROR) << "Run as " << argv[0]
               << " fields.yml input.csv custom.txt output.csv";
    return 1;
  }
  timetabler = new Timetabler();
  Parser parser(timetabler);
  parser.parseFields(std::string(argv[1]));
  parser.parseInput(std::string(argv[2]));
  if (parser.verify()) {
    LOG(INFO) << "Input is valid";
  } else {
    LOG(ERROR) << "Input is invalid";
    delete timetabler;
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
  if (solverStatus == SolverStatus::Solved ||
      solverStatus == SolverStatus::HighLevelFailed) {
    timetabler->writeOutput(std::string(argv[4]));
  }
  delete timetabler;
  return 0;
}
