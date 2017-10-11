#include "fields/is_minor.h"

#include "global.h"

IsMinor::IsMinor(MinorType minorType) {
    this->minorType = minorType;
}

IsMinor::IsMinor(bool isMinor) {
	this->minorType = (isMinor) : MinorType::isMinor ? MinorType::isNotMinor;
}

FieldType IsMinor::getType() {
    return FieldType::isMinor;
}