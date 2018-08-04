/** @file */

#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "global.h"
#include "mtl/Vec.h"
#include <string>
#include <vector>

using namespace Minisat;

namespace Utils {

/**
 * @brief      Converts a std::vector to a Minisat::vec.
 *
 * @param[in]  inputs  The input vector
 *
 * @tparam     T       The type of the vector
 *
 * @return     The resultant vec
 */
template <typename T> vec<T> convertVectorToVec(std::vector<T> inputs) {
    vec<T> result(inputs.size());
    for (int i = 0; i < inputs.size(); i++) {
        result[i] = inputs[i];
    }
    return result;
}

/**
 * @brief      Converts a Minisat::vec to a std::vector.
 *
 * @param[in]  inputs  The input vec
 *
 * @tparam     T       The type of the vec
 *
 * @return     The resultant vector
 */
template <typename T> std::vector<T> convertVecToVector(vec<T> inputs) {
    std::vector<T> result(inputs.size());
    for (int i = 0; i < inputs.size(); i++) {
        result[i] = inputs[i];
    }
    return result;
}

/**
 * @brief      Converts given data and size, a Minisat::vec to a std::vector.
 *
 * @param      data  The data pointer
 * @param[in]  size  The size
 *
 * @tparam     T     The type of the vec
 *
 * @return     The resultant vector
 */
template <typename T> std::vector<T> convertVecDataToVector(T *data, int size) {
    std::vector<T> result(size);
    for (int i = 0; i < size; i++) {
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
    for (int i = 0; i < inputs.size(); i++) {
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
    for (int i = 0; i < inputs.size(); i++) {
        std::vector<T> nextInsert = flattenVector<T>(inputs[i]);
        result.insert(result.end(), nextInsert.begin(), nextInsert.end());
    }
    return result;
}

std::string getFieldTypeName(FieldType fieldType);

std::string getFieldName(FieldType fieldType, int index, Data &data);

} // namespace Utils

#endif