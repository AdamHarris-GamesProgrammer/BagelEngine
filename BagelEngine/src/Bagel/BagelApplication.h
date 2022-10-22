#pragma once
#include "Core.h"
#include "Events/Event.h"

#include "Bagel/Events/ApplicationEvent.h"
#include "Bagel/Events/KeyEvent.h"
#include "Bagel/Events/MouseEvent.h"

#include "Window.h"

namespace Bagel {
	class BG_API BagelApplication
	{
	public:
		BagelApplication();
		virtual ~BagelApplication();

		void Run();


		void OnEvent(Event& event);
	private:

		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> _pWindow;
		bool _running = true;
	};

	//Defined in client app
	BagelApplication* CreateApplication();
}



