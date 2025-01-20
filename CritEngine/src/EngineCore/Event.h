#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>
#include "Logging/Logger.h"

namespace Engine {

	using FunctionPointer = std::function<void(void)>; // Make it so we can type all that less

	std::vector<FunctionPointer> functions;
	std::unordered_map<std::string, int> nameMap; // Names stored for manual triggering of functions

	// Nifty preprocessor thing so we don't have to rewrite the lambda function over and over
	#define AddListener(a,b) addListener(a, [this]() {this->b(); }); 

	void addListener(std::string funcName, FunctionPointer functionPointer) {
		functions.push_back(functionPointer);
		nameMap[funcName] = functions.size() - 1;
	}

	void emit(std::string funcName) {

		for (auto& pair : nameMap) {
			if (pair.first == funcName) {
				FunctionPointer func = functions[nameMap[funcName]];
				func();
				return;
			}
		}
		// If there is no matches to the function name
		LogWarning("Event", funcName + " is not a valid function or could not be found");

	}
}
