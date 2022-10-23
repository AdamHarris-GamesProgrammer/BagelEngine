#pragma once

#include "Bagel/Core.h"
#include "Bagel/Events/Event.h"

namespace Bagel {
	class BG_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		//Called on attachment to the layer stack
		virtual void OnAttach(){}

		//Called on detachment from the layer stack
		virtual void OnDetach(){}

		//Called per frame
		virtual void OnUpdate(){}

		//Called for each event
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return _debugName; }

	protected:
		std::string _debugName;
	};
}
