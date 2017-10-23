#ifndef CUSTOM_PARSER_H
#define CUSTOM_PARSER_H

#include "constraint_adder.h"
#include "time_tabler.h"
#include <vector>
#include <string>

/**
 * @brief      Class for field values types.
 */
enum class FieldValuesType {
    COURSE, INSTRUCTOR, PROGRAM, ISMINOR, SEGMENT, CLASSROOM, SLOT
};

/**
 * @brief      Struct for the type used by actions in the parser.
 */
struct Object {
    bool isNot;
    bool classSame;
    bool slotSame;
    int integer;
    TimeTabler* timeTabler;
    ConstraintAdder* constraintAdder;
    ConstraintEncoder* constraintEncoder;

    FieldValuesType fieldType;
    std::vector<int> courseValues;
    std::vector<int> instructorValues;
    std::vector<int> programValues;
    std::vector<int> isMinorValues;
    std::vector<int> segmentValues;
    std::vector<int> classValues;
    std::vector<int> slotValues;

    Clauses constraint;
    std::vector<Clauses> constraintAnds;
    std::vector<Clauses> constraintVals;
};

void parseCustomConstraints(std::string, ConstraintEncoder*, TimeTabler*);

#endif