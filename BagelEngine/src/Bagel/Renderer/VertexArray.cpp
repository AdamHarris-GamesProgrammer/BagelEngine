#include "bgpch.h"
#include "Bagel/Renderer/VertexArray.h"

#include "Bagel/Renderer/Renderer.h"

#include "Bagel/Platform/OpenGL/OpenGLVertexArray.h"

namespace Bagel {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::None: BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}