#include "fields/is_minor.h"

#include <string>
#include "global.h"

/**
 * @brief      Constructs the IsMinor object.
 *
 * @param[in]  minorType  The minor type of the Course
 */
IsMinor::IsMinor(MinorType minorType) {
    this->minorType = minorType;
}

/**
 * @brief      Constructs the IsMinor object.
 *
 * @param[in]  isMinor  Indicates if the Course is a minor course
 */
IsMinor::IsMinor(bool isMinor) {
    if(isMinor) {
        this->minorType = MinorType::isMinorCourse;
    }
    else {
        this->minorType = MinorType::isNotMinorCourse;
    }
}


/**
 * @brief      Gets the type under the FieldType enum.
 *
 * @return     A member of the FieldType enum, which is FieldType::isMinor
 */
FieldType IsMinor::getType() {
    return FieldType::isMinor;
}

/**
 * @brief      Gets the minor type of the Course.
 *
 * @return     The minor type.
 */
MinorType IsMinor::getMinorType() {
    return minorType;
}


/**
 * @brief      Gets the type name, which is "Minor Type".
 *
 * @return     The string "Minor Type"
 */
std::string IsMinor::getTypeName() {
    return "Minor Type";
}

/**
 * @brief      Gets whether the Course is a minor course or not.
 *
 * @return     "Yes" if it is a minor course, "No" if not
 */
std::string IsMinor::getName() {
    if(minorType == MinorType::isMinorCourse) {
        return "Yes";
    }
    return "No";
}