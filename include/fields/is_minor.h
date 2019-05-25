/** @file */

#ifndef IS_MINOR_H
#define IS_MINOR_H

#include "fields/field.h"
#include "global.h"

/**
 * @brief      Enum to represent the types of "Is Minor".
 *
 * A Course can either be a minor course or not. If it is, the
 * value under this enum would be isMinorCourse, and otherwise,
 * it would be isNotMinorCourse.
 */
enum class MinorType {
  /**
   * Value when the Course is a minor course
   */
  isMinorCourse,
  /**
   * Value when the Course is not a minor course
   */
  isNotMinorCourse
};

/**
 * @brief      Class for "is minor".
 *
 * This represents whether a Course is a minor course or not.
 */
class IsMinor : public Field {
 private:
  /**
   * Stores the minor type of the course, which is whether the
   * course is a minor course or not
   */
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
