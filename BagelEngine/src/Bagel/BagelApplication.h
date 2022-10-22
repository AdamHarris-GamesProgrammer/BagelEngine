#pragma once
#include "Core.h"
#include "Events/Event.h"

#include "Window.h"

namespace Bagel {
	class BG_API BagelApplication
	{
	public:
		BagelApplication();
		virtual ~BagelApplication();

		void Run();

	private:
		std::unique_ptr<Window> _pWindow;
		bool _running = true;
	};

	//Defined in client app
	BagelApplication* CreateApplication();
}



