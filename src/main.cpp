#include <getopt.h>
#include <iostream>
#include "constraint_adder.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "custom_parser.h"
#include "global.h"
#include "global_vars.h"
#include "mtl/Vec.h"
#include "parser.h"

void display_meta() {
  std::cout << "Timetabler\n"
               "Version: 0.3.0+git\n\n"
               "A highly customizable timetabling software for educational "
               "institutions that encodes timetabling constraints as a SAT "
               "formula and solves them using a MaxSAT solver."
            << std::endl;
}

const struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                      {"input", required_argument, 0, 'i'},
                                      {"fields", required_argument, 0, 'f'},
                                      {"custom", required_argument, 0, 'c'},
                                      {"output", required_argument, 0, 'o'},
                                      {"version", no_argument, 0, 'v'},
                                      {0, 0, 0, 0}};

const std::string option_desc[] = {"display this help",
                                   "Input file",
                                   "Fields file",
                                   "Custom file",
                                   "Output file",
                                   "display version",
                                   ""};

void display_help() {
  display_meta();
  std::cout << "\nUsage:\n";
  std::cout << " timetabler"
               " -i|--input <input_file>"
               " -f|--fields <fields_file>"
               " [-c|--custom <custom_constraints_file>]"
               " -o|--output <output_file>"
               "\n\n";
  std::cout << "Options:\n";
  for (int i = 0; long_options[i].name != 0; i++) {
    if (long_options[i].val)
      std::cout << '-' << char(long_options[i].val) << "\t";
    std::cout << "--" << long_options[i].name << "\t" << option_desc[i] << "\n";
  }
}

void display_error(std::string err) {
  std::cout << err << std::endl;
  std::cout << "Use --help flag for supported options." << std::endl;
  exit(1);
}

Timetabler *timetabler;

int main(int argc, char *const *argv) {
  std::string input_file, fields_file, custom_file, output_file;

  while (1) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "hi:f:c:o:v", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        break;
      case 'h':
        display_help();
        exit(0);
      case 'v':
        display_meta();
        exit(0);
      case 'i':
        input_file = std::string(optarg);
        break;
      case 'f':
        fields_file = std::string(optarg);
        break;
      case 'c':
        custom_file = std::string(optarg);
        break;
      case 'o':
        output_file = std::string(optarg);
        break;
      case '?':
        break;
      default:
        display_error("Unrecognised argument: " + std::to_string(c));
    }
  }

  if (input_file == "") {
    display_error("Input file is required.");
  }

  if (fields_file == "") {
    display_error("Fields file is required.");
  }

  if (output_file == "") {
    display_error("Output file is required.");
  }

  if (optind < argc) {
    display_error("Unrecognised argument: " + std::string(argv[optind]));
  }

  timetabler = new Timetabler();
  Parser parser(timetabler);
  parser.parseFields(fields_file);
  parser.parseInput(input_file);
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
  if (custom_file != "") {
    parseCustomConstraints(custom_file, &encoder, timetabler);
  }
  timetabler->addHighLevelClauses();
  timetabler->addExistingAssignments();
  SolverStatus solverStatus = timetabler->solve();
  timetabler->printResult(solverStatus);
  if (solverStatus == SolverStatus::Solved ||
      solverStatus == SolverStatus::HighLevelFailed) {
    timetabler->writeOutput(output_file);
  }
  delete timetabler;
  return 0;
}
