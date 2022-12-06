#include "bgpch.h"
#include "Bagel/Renderer/VertexArray.h"

#include "Bagel/Renderer/Renderer.h"

#include "Bagel/Platform/OpenGL/OpenGLVertexArray.h"

namespace Bagel {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}