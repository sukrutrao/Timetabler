#ifndef CUSTOM_PARSER_H
#define CUSTOM_PARSER_H

#include "constraint_adder.h"
#include "time_tabler.h"
#include <vector>
#include <string>

struct Object {
    bool isNot;
    bool takeFieldValues;
    bool takeClassValues;
    bool takeSlotValues;
    int integer;
    TimeTabler* timeTabler;
    ConstraintAdder* constraintAdder;

    std::string fieldType;
    std::vector<int> fieldValues;
    std::vector<int> classValues;
    std::vector<int> slotValues;

    Clauses constraint;
    std::vector<Clauses> constraintAnds;
    std::vector<Clauses> constraintVals;
};

void parseCustomConstraints(ConstraintAdder*, TimeTabler*);

#endif