#include "bgpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Bagel {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: _pWindowHandle(windowHandle)
	{
		BG_CORE_ASSERT(_pWindowHandle, "Window handle is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_pWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BG_CORE_ASSERT(status, "Failed to initialize Glad!");

		BG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		BG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		BG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_pWindowHandle);
	}
}