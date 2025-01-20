#pragma once

#include <iostream>
#include <vector>

class Listener {
public:
	virtual void onNotify(const std::string& event) = 0; // To be overriden with our own behavior when inheriting

};

class Emitter {

private:
	std::vector<Listener*> listeners; // A dynamic list of listeners. 

public:
	void addListener(Listener* listener) {
		listeners.push_back(listener);
	}
	void removeListener(Listener* listener) {
		//TODO: Ability to remove a listener
	}
	void notify(const std::string& event) {
		for (Listener* listener : listeners) { 
			listener->onNotify(event);
		}
	}

	~Emitter() {
		// I'm not sure if the vector will linger in memory, but if so, we might can clean it up here
	}
};

