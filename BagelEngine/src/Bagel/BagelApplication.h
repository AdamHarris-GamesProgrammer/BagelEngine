#pragma once
#include "Core.h"
#include "Events/Event.h"

#include "Bagel/Events/ApplicationEvent.h"
#include "Bagel/Events/KeyEvent.h"
#include "Bagel/Events/MouseEvent.h"

#include "Window.h"
#include "Bagel/LayerStack.h"
#include "Bagel/Events/Event.h"
#include "Bagel/Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace Bagel {
	class BG_API BagelApplication
	{
	public:
		BagelApplication();
		virtual ~BagelApplication();

		void Run();


		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *_pWindow; }

		inline static BagelApplication& Get() { return *_instance; }
	private:

		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> _pWindow;
		bool _running = true;

		LayerStack _layerStack;

		ImGuiLayer* _pImGuiLayer;

		static BagelApplication* _instance;
	};

	//Defined in client app
	BagelApplication* CreateApplication();
}