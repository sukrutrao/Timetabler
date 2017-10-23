# Time Tabling

This project generates a time table given a set of inputs and constraints. It encodes constraints of the time tabling problem as a SAT formula in the Conjunctive Normal Form, and then calls a MaxSAT solver to solve the problem.

## Installation
### Prerequisities
The following software are prerequisites for this program:
* Open WBO 2.0
This is the MaxSAT solver used to solve the encoded formula. The code can be downloaded from https://github.com/sat-group/open-wbo/tree/f193a3bd802551b13d6424bc1baba6ad35ec6ba6. Also see http://sat.inesc-id.pt/open-wbo/ for details.
* yaml-cpp 0.5.3
This is used to parse valid field values and weight inputs. Download and install from https://github.com/jbeder/yaml-cpp/releases/tag/yaml-cpp-0.5.3.
* CSVparser
This is used to parse the CSV containing the input data. Download from https://github.com/MyBoon/CSVparser/tree/540e3e2f46b77ea8178f90910a165695cbb6cc12.
* PEGTL 2.2.0
This is used to parse custom constraints. Download and install from https://github.com/taocpp/PEGTL/releases/tag/2.2.0.
* googletest 1.8.0
This is required if tests are to be run. Download and install from https://github.com/google/googletest/releases/tag/release-1.8.0.

`g++` with support for C++11, using the `--std=c++11` flag, is required.

### Building the project
Clone the repository, and enter it
```
$ git clone https://github.com/GoodDeeds/time-tabling.git
$ cd time-tabling
```
In the `Makefile`, edit the first two lines as per your setup:
```
OPEN_WBO_PATH = <path-to-open-wbo-directory>
CSV_PARSER_PATH = <path-to-CSVparser-directory>
```
Then, build the project
```
$ make
```
The build files are created in the `bin/` directory. The executable is `TimeTabler`, found in this directory.

To make and run tests, use `make test`, and then run the `test` executable from the `bin/` directory.

## Running the Time Tabler
To execute the program, the format is
```
$ ./TimeTabler fields.yml input.csv custom.txt output.csv
```
where
* `fields.yml` is the file containing the list of values a field can take and the weights for the constraints.
* `input.csv` is the file containing the input data.
* `custom.txt` is the file containing the list of custom constraints.
* `output.csv` is the file to which the output must be written to.

For further details, please refer to the [Project Wiki](https://github.com/GoodDeeds/time-tabling/wiki).
