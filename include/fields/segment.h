#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "fields/field.h"
#include "global.h"

class Segment : public Field{
private:
    int startSegment;
    int endSegment;
public:
    Segment(int, int);
    bool operator==(const Segment &other);
    int length();
    bool isIntersecting(const Segment &other);
    FieldType getType();
    std::string getName();
    std::string getTypeName();
};

#endif
