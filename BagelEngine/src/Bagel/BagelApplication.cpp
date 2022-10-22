#include "bgpch.h"
#include "BagelApplication.h"



#include <GLFW/glfw3.h>



namespace Bagel {

#define BIND_EVENT_FN(x) std::bind(&BagelApplication::x, this, std::placeholders::_1)

	BagelApplication::BagelApplication()
	{
		_pWindow = std::unique_ptr<Window>(Window::Create());
		_pWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		while (_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			_pWindow->OnUpdate();
		}
	}

	void BagelApplication::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		BG_TRACE(event.ToString());
	}

	bool BagelApplication::OnWindowClosed(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

}
