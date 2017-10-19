#include "utils.h"

namespace Utils {

std::string getFieldTypeName(FieldType fieldType) {
	if(fieldType == FieldType::classroom) return "Classroom";
	if(fieldType == FieldType::instructor) return "Instructor";
	if(fieldType == FieldType::isMinor) return "Minor Type";
	if(fieldType == FieldType::program) return "Programs";
	if(fieldType == FieldType::segment) return "Segment";
	if(fieldType == FieldType::slot) return "Slot";
	assert(false && "Invalid field type!");
	return "Invalid Type";
}

}