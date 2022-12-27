#include "bgpch.h"
#include "RenderCommand.h"

#include "Bagel/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Bagel {
	Scope<RendererAPI> RenderCommand::_sRendererAPI = CreateScope<OpenGLRendererAPI>();
}