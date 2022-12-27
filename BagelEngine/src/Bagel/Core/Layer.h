#pragma once

#include "Bagel/Core/Core.h"
#include "Bagel/Events/Event.h"
#include "Bagel/Core/TimeStep.h"

namespace Bagel {
	class BG_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		//Called on attachment to the layer stack
		virtual void OnAttach(){}

		//Called on detachment from the layer stack
		virtual void OnDetach(){}

		//Called per frame
		virtual void OnUpdate(Timestep ts){}

		virtual void OnImGuiRender() {}

		//Called for each event
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return _debugName; }

	protected:
		std::string _debugName;
	};
}
