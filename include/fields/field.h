/** @file */

#ifndef FIELD_H
#define FIELD_H

#include <string>
#include "global.h"

/**
 * @brief      Class for a field.
 *
 * This is a virtual base class from which all fields inherit.
 */
class Field {
 public:
  /**
   * @brief      Gets the type of the Field, as a member of the FieldType
   * enum.
   *
   * @return     The type of the Field.
   */
  virtual FieldType getType() = 0;
  /**
   * @brief      Gets the type name of the Field.
   *
   *             For example, the type name of the Field Classroom is
   * "Classroom".
   *
   * @return     The type name of the Field.
   */
  virtual std::string getTypeName() = 0;
};

#endif
