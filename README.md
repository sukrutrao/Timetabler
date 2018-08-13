# Timetabler

[![Build Status](https://travis-ci.com/GoodDeeds/Timetabler.svg?branch=development)](https://travis-ci.com/GoodDeeds/Timetabler)
[![Documentation Status](https://readthedocs.org/projects/timetabler/badge/?version=development)](https://timetabler.readthedocs.io/en/development/?badge=development)

This project generates a timetable given a set of inputs and constraints. It encodes constraints of the timetabling problem as a SAT formula in the Conjunctive Normal Form, and then calls a MaxSAT solver to solve the problem.

## Installation

**For ubuntu users:** Download deb for the latest version of Timetabler from [here](https://github.com/GoodDeeds/Timetabler/releases) and install it.

To build the project from source follow the steps below.

### Prerequisities

This project requires following tools to build.
* `cmake`
* `g++` or any other C++ compiler with C++11 support

### Dependencies

The following software are dependencies for this program:
* [**Open WBO 2.0**](https://github.com/GoodDeeds/open-wbo): Slightly modified version of the original [Open-WBO 2.0](https://github.com/sat-group/open-wbo/tree/f193a3bd802551b13d6424bc1baba6ad35ec6ba6).
* [**yaml-cpp 0.5.3**](https://github.com/jbeder/yaml-cpp/releases/tag/yaml-cpp-0.5.3)
* [**CSVparser**](https://github.com/MyBoon/CSVparser/tree/540e3e2f46b77ea8178f90910a165695cbb6cc12)
* [**PEGTL 2.2.0**](https://github.com/taocpp/PEGTL/releases/tag/2.2.0)
* [**googletest 1.8.0**](https://github.com/google/googletest/releases/tag/release-1.8.0)

### Setting up the dependencies

#### Short method

To set up all the dependencies required, simply run the following command
```bash
$ ./install_dependencies.sh
```
If the above command fails, you can try installing the individual dependencies manually by following the below instructions.

#### Long method

This describes the process of setting up each dependency.

##### Open WBO 2.0 

This needs to be built as a library. The following steps need to be followed:
* Clone the repository. `$OPEN_WBO_PATH` will be used to denote the path where the repository is cloned.
```bash
$ git clone https://github.com/GoodDeeds/open-wbo.git $OPEN_WBO_PATH
$ cd $OPEN_WBO_PATH
```
* Build as a static library
```bash
$ LIB=open-wbo make libr
```

##### CSVparser

This does not require any setup other than cloning the repository. The path where this is cloned will be referred to as `$CSVPARSER_PATH`.

##### yaml-cpp

* Download [yaml-cpp-0.5.3](https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.5.3.tar.gz) and unpack it. `$YAML_CPP_PATH` will be used to denote the path where it is unpacked.
* Build the project
```bash
$ cd $YAML_CPP_PATH
$ mkdir build && cd build
$ cmake -DCMAKE_SHARED_LIBS=ON ..
$ make
```

##### PEGTL

* Download [PEGTL 2.2.0](https://github.com/taocpp/PEGTL/archive/2.2.0.tar.gz) and unpack it. `$PEGTL_PATH` will be used to denote the path where it is unpacked.
* Build the project.
```bash
$ cd $PEGTL_PATH
$ mkdir build && cd build
$ cmake -DCMAKE_SHARED_LIBS=ON ..
$ make
```

### Building the project

* Clone the repository
```
$ git clone https://github.com/GoodDeeds/Timetabler
$ cd Timetabler
```
* Build the project. Set the cmake variables `OPEN_WBO_PATH`, `YAML_CPP_PATH`, `CSVPARSER_PATH` and `PEGTL_PATH` appropriately.
```bash
$ mkdir build && cd build
$ cmake -DOPEN_WBO_PATH="" -DYAML_CPP_PATH="" -DCSVPARSER_PATH="" -DPEGTL_PATH="" ..
$ make
```
* Install
```bash
$ make install
```

## Running the Timetabler
To execute the program, the format is
```bash
$ timetabler fields.yml input.csv custom.txt output.csv
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
