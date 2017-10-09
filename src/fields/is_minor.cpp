#include "fields/is_minor.h"

IsMinor::IsMinor(MinorType minorType) {
	this->minorType = minorType;
}

FieldType IsMinor::getType() {
	return FieldType::isMinor;
}