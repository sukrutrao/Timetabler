# Timetabler

[![Build Status](https://travis-ci.com/GoodDeeds/Timetabler.svg?branch=master)](https://travis-ci.com/GoodDeeds/Timetabler)
[![Documentation Status](https://readthedocs.org/projects/timetabler/badge/?version=master)](https://timetabler.readthedocs.io/en/development/?badge=master)
[![GitHub](https://img.shields.io/github/license/GoodDeeds/Timetabler.svg)](LICENSE)
[![GitHub release](https://img.shields.io/github/release/GoodDeeds/Timetabler.svg)](https://github.com/GoodDeeds/Timetabler/releases/tag/v0.3.0)
[![Github commits (since latest release)](https://img.shields.io/github/commits-since/GoodDeeds/Timetabler/latest.svg)](https://github.com/GoodDeeds/Timetabler/compare/v0.3.0...master)

This project generates a timetable given a set of inputs and constraints. It encodes constraints of the timetabling problem as a SAT formula in the Conjunctive Normal Form, and then calls a MaxSAT solver to solve the problem.

## Installation

**For Ubuntu/Debian users:** Download the deb for the latest version of Timetabler from [here](https://github.com/GoodDeeds/Timetabler/releases) and install it.

To build the project from source follow the steps below.

### Prerequisities

This project requires following tools to build.
* cmake (version 3.10 or higher)
* g++ or any other C++ compiler with C++11 support

### Dependencies

The following software are dependencies for this program:
* [**Open WBO 2.1**](https://github.com/sat-group/open-wbo/tree/12382e61b0230f7406900b365a4fddc73194c970)
* [**yaml-cpp 0.6.2**](https://github.com/jbeder/yaml-cpp/releases/tag/yaml-cpp-0.6.2)
* [**CSVparser**](https://github.com/MyBoon/CSVparser/tree/540e3e2f46b77ea8178f90910a165695cbb6cc12)
* [**PEGTL 2.7.0**](https://github.com/taocpp/PEGTL/releases/tag/2.7.0)
* [**GoogleTest 1.8.1**](https://github.com/google/googletest/releases/tag/release-1.8.1) (Optional, needed only for testing)

### Get the code

The most recent stable release can be downloaded from [here](https://github.com/GoodDeeds/Timetabler/releases).

To get the latest code, use

```bash
$ git clone https://github.com/GoodDeeds/Timetabler
$ cd Timetabler
```

### Setting up the dependencies

#### Short method

To set up all the dependencies required, simply run the following command
```bash
$ ./install_dependencies.sh
```

If building with tests, use instead
```bash
$ ./install_dependencies.sh --enable-tests
```

If the above command fails, you can try installing the individual dependencies manually by following the instructions below.

#### Long method

This describes the process of setting up each dependency.

##### Open WBO 2.1

This needs to be built as a library. The following steps need to be followed:
* Clone the repository. `$OPEN_WBO_PATH` will be used to denote the path where the repository is cloned.
```bash
$ git clone https://github.com/sat-group/open-wbo.git $OPEN_WBO_PATH
$ cd $OPEN_WBO_PATH
```
* Build as a static library
```bash
$ LIB=open-wbo make libr
```

##### CSVparser

This does not require any setup other than cloning the repository. The path where this is cloned will be referred to as `$CSVPARSER_PATH`.

##### yaml-cpp

* Download [yaml-cpp-0.6.2](https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.2.tar.gz) and unpack it. `$YAML_CPP_PATH` will be used to denote the path where it is unpacked.
* Build the project
```bash
$ cd $YAML_CPP_PATH
$ mkdir build && cd build
$ cmake ..
$ make
```

##### PEGTL

* Download [PEGTL 2.7.0](https://github.com/taocpp/PEGTL/archive/2.7.0.tar.gz) and unpack it. `$PEGTL_PATH` will be used to denote the path where it is unpacked.
* Build the project.
```bash
$ cd $PEGTL_PATH
$ mkdir build && cd build
$ cmake ..
$ make
```

##### GoogleTest

* Download [GoogleTest 1.8.1](https://github.com/google/googletest/releases/tag/release-1.8.1) and unpack it. `$GTEST_PATH` will be used to denote the path where it is unpacked.
* Build the project
```bash
$ cd $GTEST_PATH
$ mkdir build && cd build
$ cmake ..
$ make
```

### Building the project

* Build the project. Set the cmake variables `OPEN_WBO_PATH`, `YAML_CPP_PATH`, `CSVPARSER_PATH` and `PEGTL_PATH` appropriately.
```bash
$ mkdir build && cd build
$ cmake -DOPEN_WBO_PATH="../dependencies/open-wbo" -DYAML_CPP_PATH="../dependencies/yaml-cpp-yaml-cpp-0.6.2" -DCSVPARSER_PATH="../dependencies/CSVparser" -DPEGTL_PATH="../dependencies/PEGTL-2.7.0" -DENABLE_TESTS=Off ..
$ make timetabler
```

If building with tests, set `GTEST_PATH` appropriately and use
```bash
$ mkdir build && cd build
$ cmake -DOPEN_WBO_PATH="../dependencies/open-wbo" -DYAML_CPP_PATH="../dependencies/yaml-cpp-yaml-cpp-0.6.2" -DCSVPARSER_PATH="../dependencies/CSVparser" -DPEGTL_PATH="../dependencies/PEGTL-2.7.0" -DGTEST_PATH="../dependencies/googletest-release-1.8.1" -DENABLE_TESTS=On ..
$ make timetabler
```

* To run tests
```bash
$ make tests # Build tests
$ make test # Run tests
```
* Install
```bash
$ make install
```

## Running the Timetabler
To execute the program, use
```bash
$ timetabler fields.yml input.csv custom.txt output.csv
```
where
* `fields.yml` is the path to the file containing the list of values a field can take and the weights for the constraints. This includes list of instructors, available classrooms, weights of clauses, etc.
* `input.csv` is the path to the file containing the input data. This file contains the course data input given to the solver as a CSV file.
* `custom.txt` is the path to the file containing the list of custom constraints. This file contains the custom constraints that can be provided to the solver using the grammar provided. For the full grammar, please refer to the [Project Wiki](https://github.com/GoodDeeds/Timetabler/wiki).
* `output.csv` is the path to the file to which the output must be written to.

A detailed explanation on each file can be found in the [Project Wiki](https://github.com/GoodDeeds/Timetabler/wiki).

## Examples of Configuration files

Examples for configuration files can be found [here](https://github.com/GoodDeeds/Timetabler/blob/master/examples). This contains some examples for the field information, the input, and custom constraints to be added to the solver.

For further details and examples, please refer to the [Project Wiki](https://github.com/GoodDeeds/Timetabler/wiki).

## Documentation

The project web page is [gooddeeds.github.io/Timetabler](https://gooddeeds.github.io/Timetabler).

Code documentation for this project can be found on [Read the Docs](https://timetabler.readthedocs.io/).

## Issues

If you have any queries, suggestions, or feature requests, or would like to report any bugs or issues, please [open an issue on GitHub](https://github.com/GoodDeeds/Timetabler/issues/new).

When reporting bugs, please provide sufficient information (such as the inputs and the configuration used) to reproduce the bug.

## Contributing

To contribute to this project, please send a [pull request](https://github.com/GoodDeeds/Timetabler/compare).

## License

This software is provided under the [MIT License](https://github.com/GoodDeeds/Timetabler/blob/master/LICENSE).
