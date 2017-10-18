#ifndef IS_MINOR_H
#define IS_MINOR_H

#include "fields/field.h"
#include "global.h"

enum class MinorType {
    isMinor,
    isNotMinor
};


class IsMinor : public Field {
private:
    MinorType minorType;
public:
    IsMinor(MinorType);
    IsMinor(bool);
    FieldType getType();
};

#endif
