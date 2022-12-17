#include "bgpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Bagel/Platform/OpenGL/OpenGLShader.h"

namespace Bagel {

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI Selected!");

		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& shaderSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:		BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(shaderSrc);
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI Selected!");

		return nullptr;
	}
}