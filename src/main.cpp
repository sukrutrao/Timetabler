#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <string>
#include "constraint_adder.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "custom_parser.h"
#include "global.h"
#include "global_vars.h"
#include "mtl/Vec.h"
#include "parser.h"
#include "utils.h"
#include "version.h"

/**
 * @brief      Display information about timetabler
 *
 * @param[in]  display_desc  If true, also prints the description
 */
void display_meta(bool display_desc = false) {
  std::cout << "Timetabler version " __TIMETABLER_VERSION__ << std::endl;
  if (display_desc)
    std::cout << "\n"
              << "A highly customizable timetabling software for educational\n"
                 "institutions that encodes timetabling constraints as a SAT\n"
                 "formula and solves them using a MaxSAT solver."
              << std::endl;
}

/**
 * Options supported in command line interface
 */
const struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                      {"fields", required_argument, 0, 'f'},
                                      {"input", required_argument, 0, 'i'},
                                      {"custom", required_argument, 0, 'c'},
                                      {"output", required_argument, 0, 'o'},
                                      {"verbosity", required_argument, 0, 'b'},
                                      {"version", no_argument, 0, 'v'},
                                      {0, 0, 0, 0}};

/**
 * Descriptions of the options supported in CLI
 */
const std::string option_desc[] = {"display this help",
                                   "fields yaml file",
                                   "input csv file",
                                   "custom constraints file",
                                   "output csv file",
                                   "specify verbosity level (0-3)"
                                   "display version",
                                   ""};

/**
 * @brief      Display help for CLI options.
 *
 * @param[in]  exec  Name of the executable
 */
void display_help(std::string exec = "timetabler") {
  display_meta(true);
  std::cout << "\nUsage:\n";
  std::cout << " " << exec
            << " -i|--input <input_file>"
               " -f|--fields <fields_file>"
               " [-c|--custom <custom_constraints_file>]"
               " -o|--output <output_file>"
               "\n\n";
  std::cout << "Options:\n";
  for (int i = 0; long_options[i].name != 0; i++) {
    if (long_options[i].val)
      std::cout << '-' << char(long_options[i].val) << ", ";
    std::cout << std::left << "--" << std::setw(8) << long_options[i].name
              << "\t" << option_desc[i] << "\n";
  }
}

/**
 * @brief      Display error message when unrecognised option is given in CLI
 *
 * @param[in]  err   The error message
 */
void display_error(std::string err) {
  std::cout << err << std::endl;
  std::cout << "Use --help option to know about supported options."
            << std::endl;
  exit(1);
}

Timetabler *timetabler;

/**
 * @brief      The main function
 *
 * @param[in]  argc  Count of the cli arguments
 * @param      argv  Arguments passed through cli
 *
 * @return     Exit code when program ends
 */
int main(int argc, char *const *argv) {
  std::string input_file, fields_file, custom_file, output_file;
  unsigned verbosity = 3;

  while (1) {
    int option_index = 0;
    int c =
        getopt_long(argc, argv, "hi:f:c:o:b:v", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        break;
      case 'h':
        display_help(std::string(argv[0]));
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
      case 'b':
        verbosity = std::stoi(optarg);
        break;
      case '?':
        break;
      default:
        display_error("Unrecognised argument: " + std::to_string(c));
    }
  }

  Utils::Log::setVerbosity(verbosity);

  if (optind < argc) {
    display_error("Unrecognised argument: " + std::string(argv[optind]));
  }

  if (input_file == "" || fields_file == "" || output_file == "") {
    display_error(
        "Fields filename, input filename and output filename are required.");
  }

  timetabler = new Timetabler();
  Parser parser(timetabler);
  parser.parseFields(fields_file);
  parser.parseInput(input_file);
  if (parser.verify()) {
    LOG(INFO) << "Input is valid";
  } else {
    LOG(ERROR) << "Input is invalid";
  }
  parser.addVars();
  ConstraintEncoder encoder(timetabler);
  ConstraintAdder constraintAdder(&encoder, timetabler);
  constraintAdder.addConstraints();
  if (custom_file != "") {
    parseCustomConstraints(custom_file, &encoder, timetabler);
    LOG(INFO) << "Custom constraints parsed.";
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
