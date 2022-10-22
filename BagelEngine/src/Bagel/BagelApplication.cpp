#include "bgpch.h"
#include "BagelApplication.h"

#include "Bagel/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Bagel {
	BagelApplication::BagelApplication()
	{
		_pWindow = std::unique_ptr<Window>(Window::Create());
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

}
