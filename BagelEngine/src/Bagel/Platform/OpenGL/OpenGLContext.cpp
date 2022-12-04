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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_pWindowHandle);
	}
}