#include <vector>
#include "mtl/Vec.h"

namespace Utils {

template <typename T>
vec<T> convertVectorToVec(std::vector<T>);

template <typename T>
std::vector<T> convertVecToVector(vec<T>);

}