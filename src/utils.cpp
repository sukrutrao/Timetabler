#include "utils.h"

#include <vector>
#include "mtl/Vec.h"

using namespace Minisat;

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

template <typename T>
std::vector<T> flattenVector(std::vector<std::vector<T>> inputs) {
	std::vector<T> result;
	result.clear();
	for(int i = 0; i < inputs.size(); i++) {
		result.insert(result.end(), inputs[i].begin(), inputs[i].end());
	}
	return result;
}

template<typename T>
std::vector<T> flattenVector(std::vector<std::vector<std::vector<T>>> inputs) {
	std::vector<T> result;
	result.clear();
	for(int i = 0; i < inputs.size(); i++) {
		std::vector<T> nextInsert = flattenVector<T>(inputs[i]);
		result.insert(result.end(), nextInsert.begin(), nextInsert.end());
	}
	return result;
}

}