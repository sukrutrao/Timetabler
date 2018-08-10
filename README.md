# Timetabler
[![Build Status](https://travis-ci.com/GoodDeeds/Timetabler.svg?branch=development)](https://travis-ci.com/GoodDeeds/Timetabler)
[![Documentation Status](https://readthedocs.org/projects/timetabler/badge/?version=stable)](https://timetabler.readthedocs.io/en/stable/?badge=stable)

This project generates a time table given a set of inputs and constraints. It encodes constraints of the time tabling problem as a SAT formula in the Conjunctive Normal Form, and then calls a MaxSAT solver to solve the problem.

## Installation
### Prerequisities
The following software are prerequisites for this program:
* **Open WBO 2.0** : 
This is the MaxSAT solver used to solve the encoded formula. The code can be downloaded from [here](https://github.com/sat-group/open-wbo/tree/f193a3bd802551b13d6424bc1baba6ad35ec6ba6). Setup instructions are provided below. Also see the [project website](http://sat.inesc-id.pt/open-wbo/) for details.
* **yaml-cpp 0.5.3** : 
This is used to parse valid field values and weight inputs. Download and install from [here](https://github.com/jbeder/yaml-cpp/releases/tag/yaml-cpp-0.5.3).
* **CSVparser** : 
This is used to parse the CSV containing the input data. Download from [here](https://github.com/MyBoon/CSVparser/tree/540e3e2f46b77ea8178f90910a165695cbb6cc12). Setup instructions are provided below.
* **PEGTL 2.2.0** : 
This is used to parse custom constraints. Download and install from [here](https://github.com/taocpp/PEGTL/releases/tag/2.2.0).
* **googletest 1.8.0** : 
This is required if tests are to be run. Download and install from [here](https://github.com/google/googletest/releases/tag/release-1.8.0).

`g++` with support for C++11, using the `--std=c++11` flag, is required.

### Setting up the dependencies
This section explains the setup of Open WBO 2.0 and CSVparser. Installation instructions of other dependencies can be found with the code.

**Open WBO 2.0** : 

This needs to be built as a library. The following steps need to be followed:
* Clone the repository. `$OPENWBO_ROOT` will be used to denote the path where the repository is cloned.
```
$ git clone https://github.com/sat-group/open-wbo.git $OPENWBO_ROOT
$ cd $OPENWBO_ROOT
```
* Build as a static library
```
$ LIB=open-wbo make libr
```

**CSVparser**:

This does not require any setup other than cloning the repository. The path where this is cloned will be referred to as `$CSVPARSER_ROOT`.

### Building the project
* Clone the repository
```
$ git clone https://github.com/GoodDeeds/Timetabler
$ cd Timetabler
```
* In the `Makefile`, edit the first two lines as per your setup:
```
OPEN_WBO_PATH = $OPENWBO_ROOT
CSV_PARSER_PATH = $CSVPARSER_ROOT
```
* Then, build the project
```
$ make
```
The build files are created in the `bin/` directory. The executable is `Timetabler`, found in this directory.

To make and run tests, use `make test`, and then run the `test` executable from the `bin/` directory.

## Running the Timetabler
To execute the program, the format is
```
$ ./Timetabler fields.yml input.csv custom.txt output.csv
```
where
* `fields.yml` is the path to the file containing the list of values a field can take and the weights for the constraints.
* `input.csv` is the path to the file containing the input data.
* `custom.txt` is the path to the file containing the list of custom constraints.
* `output.csv` is the path to the file to which the output must be written to.

A detailed explanation on each file can be found in the [Project Wiki](https://github.com/GoodDeeds/Timetabler/wiki).

## Examples of Configuration files

This contains some examples for providing the fields information, the input, and adding custom constraints to the solver.

The structure of this directory is as follows:
* **custom-constraints** : 
This contains some examples of custom constraints that can be provided to the solver using the grammar provided. For seeing the grammar, please refer to the [Project Wiki](https://github.com/GoodDeeds/Timetabler/wiki).
* **field-input** : 
This contains some examples of the way field inputs can be given to the solver. This includes list of instructors, available classrooms, weights of clauses, etc.
* **input** : 
This contains some examples of the course data input given to the solver as a CSV file.

For further details and examples, please refer to the [Project Wiki](https://github.com/GoodDeeds/Timetabler/wiki).

## License

This software is provided under the [MIT License](LICENSE).
