#include "bgpch.h"
#include "RenderCommand.h"

#include "Bagel/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Bagel {
	RendererAPI* RenderCommand::_sRendererAPI = new OpenGLRendererAPI;
}