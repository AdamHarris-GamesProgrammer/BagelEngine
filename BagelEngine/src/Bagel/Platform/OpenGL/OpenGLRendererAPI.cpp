#include "bgpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "OpenGLError.h"

namespace Bagel {
	void OpenGLRendererAPI::Init()
	{
		BG_PROFILE_FUNCTION();

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		GLCall(glEnable(GL_DEPTH_TEST));
	}


	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		BG_PROFILE_RENDERER_FUNCTION();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		GLCall(glViewport(x, y, width, height));
	}

}