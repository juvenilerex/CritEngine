#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include "Logging/Logger.h"

class Listener {
public:
	virtual void onNotify(const std::string& event) = 0; // To be overriden with our own behavior when inheriting

};

class Emitter {

	using FunctionPointer = std::function<void(void)>; // Make it so we can type all that less

private:
	std::vector<Listener*> listeners; // A dynamic list of listeners. 
	std::vector<FunctionPointer> functions;
	std::unordered_map<std::string, int> nameMap;

public:
	void addListener(Listener* listener) {
		listeners.push_back(listener);
		
	}
	void removeListener(Listener* listener) {
		//TODO: Ability to remove a listener
	}

	// Stores functions to be called later
	void addFunction(std::string funcName, FunctionPointer functionPointer) {

		functions.push_back(functionPointer);
		nameMap[funcName] = functions.size() - 1;

	}

	void notify(const std::string& event) {

		for (Listener* listener : listeners) { 
			listener->onNotify(event);
		}
	}

	// Trigger a function by name
	void triggerFunction(std::string event) {

		FunctionPointer func = functions[nameMap[event]];
		func();

	}

	// Trigger all stored functions
	void triggerAll() {

		for (FunctionPointer func : functions) {
			func();
		}

	}

	~Emitter() {
		// I'm not sure if the vector will linger in memory, but if so, we might can clean it up here
	}
};

