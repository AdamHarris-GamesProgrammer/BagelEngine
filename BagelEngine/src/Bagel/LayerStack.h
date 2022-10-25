#pragma once

#include "Bagel/Core.h"
#include "Layer.h"

#include "bgpch.h"

namespace Bagel {
	//Wrapper class for a std::vector<Layer*> 
	class BG_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		//Pushes a layer into the layer stack at the last layer but before the overlays
		void PushLayer(Layer* layer);

		//Pushes an overlay (Puts it at the very back of the stack
		void PushOverlay(Layer* overlay);

		//Pops a layer from the stack. DOES NOT DELETE LAYER
		void PopLayer(Layer* layer);

		//Pops a overlay from the stack. DOES NOT DELETE LAYER
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return _layers.begin(); }
		std::vector<Layer*>::iterator end() { return _layers.end(); }

	private:
		std::vector<Layer*> _layers;
		unsigned int _layerInsertIndex = 0;

	};
}
