#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "mtl/Vec.h"

namespace Utils {

template <typename T>
vec<T> convertVectorToVec(std::vector<T>);

template <typename T>
std::vector<T> convertVecToVector(vec<T>);

template <typename T>
std::vector<T> flattenVector(std::vector<std::vector<T>>);

template <typename T>
std::vector<T> flattenVector(std::vector<std::vector<std::vector<T>>>);

}

#endif