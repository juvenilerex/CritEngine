#pragma once

#include "Core.h"
#include "Layer.h"
#include <vector>

namespace Engine {

	class ENGINE_API LayerStack {

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
		
	private:
		std::vector<Layer*> layers;
		std::vector<Layer*>::iterator layerInsert;
		
	};

}