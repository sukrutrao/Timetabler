#include "fields/is_minor.h"

#include "global.h"

IsMinor::IsMinor(MinorType minorType) {
    this->minorType = minorType;
}

FieldType IsMinor::getType() {
    return FieldType::isMinor;
}