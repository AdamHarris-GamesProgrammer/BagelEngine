#include "bgpch.h"
#include "LayerStack.h"

namespace Bagel {
	LayerStack::LayerStack()
	{
		
	}

	LayerStack::~LayerStack()
	{
		//Delete all layers
		for (Layer* layer : _layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		//Adds the layer to the back of the layer section of the layer stack
		_layers.emplace(_layers.begin() + _layerInsertIndex , layer);
		_layerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		//Adds overlay to back of the stack
		_layers.emplace_back(overlay);
		overlay->OnAttach();
	}


	void LayerStack::PopLayer(Layer* layer)
	{
		//Find and erase the layer
		auto it = std::find(_layers.begin(), _layers.end(), layer);
		if (it != _layers.end()) {
			_layers.erase(it);
			_layerInsertIndex--;
			layer->OnDetach();
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		//Find and erase the overlay
		auto it = std::find(_layers.begin(), _layers.end(), overlay);
		if (it != _layers.end()) {
			_layers.erase(it);
			overlay->OnDetach();
		}
	}

}
