#include "bgpch.h"
#include "Window.h"

#include "Bagel/Platform/Windows/WindowsWindow.h"

namespace Bagel {

	Window* Window::Create(const WindowProps& properties) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			case RendererAPI::API::OpenGL: return new WindowsWindow(properties);
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}

