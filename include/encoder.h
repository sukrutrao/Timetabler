#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include "global.h"

class Encoder {
private:
    std::vector<std::vector<std::vector<Var>>> vars;
    TimeTabler *timeTabler;
    const int courseCount;
    const int fieldCount;
public:
    Encoder(std::vector<std::vector<std::vector<Var>>>,const int,const int);
//  Clauses createClauses(int, int, std::function<bool(int, int, FieldType)>);
    Clauses hasSameFieldTypeAndValue(int, int, FieldType);
    Clauses hasFieldType(int, FieldType); // is this needed?
    Clauses hasSameFieldType(int, int, FieldType); // is this needed?
    Clauses notIntersectingTime(int, int);
    Clauses notIntersectingTimeField(int, int, FieldType);
    Clauses hasExactlyOneFieldValueTrue(int, FieldType);
    Clauses hasAtLeastOneFieldValueTrue(int, FieldType);
    Clauses hasAtMostOneFieldValueTrue(int, FieldType);
};

#endif