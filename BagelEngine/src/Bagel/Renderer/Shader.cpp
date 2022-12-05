#include "bgpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Bagel/Platform/OpenGL/OpenGLShader.h"

//#include <glad/glad.h>

namespace Bagel {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::None: BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
		}
	}
}