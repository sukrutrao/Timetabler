#include "utils.h"

namespace Utils {

/**
 * @brief      Gets the field type name as a string.
 * 
 * For example, the name of FieldType::instructor is "Instructor".
 *
 * @param[in]  fieldType  The FieldType member
 *
 * @return     The field type name.
 */
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