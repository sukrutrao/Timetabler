#ifndef IS_MINOR_H
#define IS_MINOR_H

#include "fields/field.h"
#include "global.h"

enum MinorType {
    isMinorCourse,
    isNotMinorCourse
};


class IsMinor : public Field {
private:
    MinorType minorType;
public:
    IsMinor(MinorType);
    IsMinor(bool);
    FieldType getType();
    MinorType getMinorType();
    std::string getTypeName();
    std::string getName();
};

#endif
