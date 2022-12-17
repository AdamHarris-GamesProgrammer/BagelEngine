#include "bgpch.h"
#include "Texture.h"

#include "Bagel/Core.h"

#include <string>

#include "Renderer.h"

#include "Bagel/Platform/OpenGL/OpenGLTexture.h"

namespace Bagel {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:		BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}