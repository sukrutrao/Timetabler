#include "utils.h"

#include <vector>
#include "mtl/Vec.h"

namespace Utils {

template <typename T>
vec<T> convertVectorToVec(std::vector<T> inputs) {
	vec<T> result(inputs.size());
	for(int i = 0; i < inputs.size(); i++) {
		result[i] = inputs[i];
	}
	return result;
}

template <typename T>
std::vector<T> convertVecToVector(vec<T> inputs) {
	std::vector<T> result(inputs.size());
	for(int i = 0; i < inputs.size(); i++) {
		result[i] = inputs[i];
	}
	return result;	
}

}