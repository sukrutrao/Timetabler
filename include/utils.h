#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include "mtl/Vec.h"
#include "global.h"

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
std::vector<T> convertVecDataToVector(T *data, int size) {
	std::vector<T> result(size);
	for(int i = 0; i < size; i++) {
		result[i] = data[i];
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

std::string getFieldTypeName(FieldType fieldType);

}

#endif