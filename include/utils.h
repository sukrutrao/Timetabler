/** @file */

#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>
#include <vector>
#include "data.h"
#include "global.h"
#include "mtl/Vec.h"

using namespace NSPACE;

namespace Utils {

/**
 * @brief      Converts a std::vector to a NSPACE::vec.
 *
 * @param[in]  inputs  The input vector
 *
 * @tparam     T       The type of the vector
 *
 * @return     The resultant vec
 */
template <typename T>
vec<T> convertVectorToVec(std::vector<T> inputs) {
  vec<T> result(inputs.size());
  for (unsigned i = 0; i < inputs.size(); i++) {
    result[i] = inputs[i];
  }
  return result;
}

/**
 * @brief      Converts a NSPACE::vec to a std::vector.
 *
 * @param[in]  inputs  The input vec
 *
 * @tparam     T       The type of the vec
 *
 * @return     The resultant vector
 */
template <typename T>
std::vector<T> convertVecToVector(vec<T> inputs) {
  std::vector<T> result(inputs.size());
  for (unsigned i = 0; i < inputs.size(); i++) {
    result[i] = inputs[i];
  }
  return result;
}

/**
 * @brief      Converts given data and size, a NSPACE::vec to a std::vector.
 *
 * @param      data  The data pointer
 * @param[in]  size  The size
 *
 * @tparam     T     The type of the vec
 *
 * @return     The resultant vector
 */
template <typename T>
std::vector<T> convertVecDataToVector(T *data, unsigned size) {
  std::vector<T> result(size);
  for (unsigned i = 0; i < size; i++) {
    result[i] = data[i];
  }
  return result;
}

/**
 * @brief      Converts a two dimensional vector to a one dimensional vector.
 *
 * This is done by concatenating all the rows.
 *
 * @param[in]  inputs  The input vector
 *
 * @tparam     T       The type of the vector
 *
 * @return     The resultant vector
 */
template <typename T>
std::vector<T> flattenVector(std::vector<std::vector<T>> inputs) {
  std::vector<T> result;
  result.clear();
  for (unsigned i = 0; i < inputs.size(); i++) {
    result.insert(result.end(), inputs[i].begin(), inputs[i].end());
  }
  return result;
}

/**
 * @brief      Converts a three dimensional vector to a one dimensional vector.
 *
 * This is done by concatenating all the rows.
 *
 * @param[in]  inputs  The input vector
 *
 * @tparam     T       The type of the vector
 *
 * @return     The resultant vector
 */
template <typename T>
std::vector<T> flattenVector(std::vector<std::vector<std::vector<T>>> inputs) {
  std::vector<T> result;
  result.clear();
  for (unsigned i = 0; i < inputs.size(); i++) {
    std::vector<T> nextInsert = flattenVector<T>(inputs[i]);
    result.insert(result.end(), nextInsert.begin(), nextInsert.end());
  }
  return result;
}

std::string getFieldTypeName(FieldType fieldType);
std::string getPredefinedConstraintName(const PredefinedClauses clauseType);

std::string getFieldName(FieldType fieldType, int index, Data &data);

}  // namespace Utils

enum class Severity { EMPTY, INFO, WARNING, ERROR };

// Reference: https://stackoverflow.com/a/2179782/, Evan Terran
// (https://stackoverflow.com/users/13430/evan-teran), CC-BY-SA 3.0

/**
 * @brief      Perform logging
 */
class Log {
 public:
  Log(Severity severity = Severity::EMPTY);
  ~Log();
  template <class T>
  Log &operator<<(const T &input) {
    ss << input;
    return *this;
  }

 private:
  std::ostringstream ss;
  Severity severity;
  int getSeverityCode(Severity);
  std::string getSeverityIdentifier(Severity);
};

#endif
