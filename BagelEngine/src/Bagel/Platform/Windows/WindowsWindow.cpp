#include "bgpch.h"
#include "WindowsWindow.h"

#include "Bagel/Events/ApplicationEvent.h"
#include "Bagel/Events/KeyEvent.h"
#include "Bagel/Events/MouseEvent.h"

#include "Bagel/Platform/OpenGL/OpenGlContext.h"

#include <GLFW/glfw3.h>

namespace Bagel {
	static bool _sGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		BG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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

			glfwSetErrorCallback(GLFWErrorCallback);

			_sGLFWInitialized = true;
		}

		_pWindow = glfwCreateWindow((int)props.width, (int)props.height, _data.title.c_str(), nullptr, nullptr);
		_pContext = new OpenGLContext(_pWindow);
		_pContext->Init();


		glfwSetWindowUserPointer(_pWindow, &_data);
		SetVSync(true);

		//Set GLFW Callbacks
		glfwSetWindowSizeCallback(_pWindow, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent e(width, height);
			data.eventCallback(e);
			});

		glfwSetWindowCloseCallback(_pWindow, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.eventCallback(e);
			});

		glfwSetKeyCallback(_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent e(key, 0);
				data.eventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(key);
				data.eventCallback(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent e(key, 1);
				data.eventCallback(e);
				break;
			}
			}
			});

		glfwSetCharCallback(_pWindow, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.eventCallback(event);

			});

		glfwSetMouseButtonCallback(_pWindow, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(button);
				data.eventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(button);
				data.eventCallback(e);
				break;
			}
			}
			});

		glfwSetScrollCallback(_pWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.eventCallback(e);

			});

		glfwSetCursorPosCallback(_pWindow, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent e((float)x, (float)y);
			data.eventCallback(e);

			});
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		_pContext->SwapBuffers();
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


