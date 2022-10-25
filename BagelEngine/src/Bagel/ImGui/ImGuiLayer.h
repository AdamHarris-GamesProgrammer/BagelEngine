#pragma once

#include "Bagel/Layer.h"

#include "Bagel/Events/ApplicationEvent.h"
#include "Bagel/Events/KeyEvent.h"
#include "Bagel/Events/MouseEvent.h"

namespace Bagel {
	class BG_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();


	private:

	private:
		float _time = 0.0f;
	};
}
