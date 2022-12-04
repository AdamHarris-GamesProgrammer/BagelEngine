#pragma once
#include "Bagel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Bagel {
	class BG_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		// Inherited via GraphicsContext
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* _pWindowHandle;
	};
}





