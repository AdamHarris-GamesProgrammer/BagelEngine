#include "bgpch.h"
#include "WindowsWindow.h"

namespace Bagel {
	static bool _sGLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		_data.title = props.title;
		_data.width = props.width;
		_data.height = props.height;

		BG_CORE_INFO("Creating Window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!_sGLFWInitialized) {
			int success = glfwInit();
			BG_CORE_ASSERT(success, "Could not initialize GLFW!");

			_sGLFWInitialized = true;
		}

		_pWindow = glfwCreateWindow((int)props.width, (int)props.height, _data.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(_pWindow);
		glfwSetWindowUserPointer(_pWindow, &_data);
		SetVSync(true);
	}



	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(_pWindow);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		_data.vsync = enabled;
	}



	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(_pWindow);
	}
}


