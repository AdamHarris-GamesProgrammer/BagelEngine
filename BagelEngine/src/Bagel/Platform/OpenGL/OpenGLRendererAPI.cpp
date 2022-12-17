#include "bgpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "OpenGLError.h"

namespace Bagel {
	void OpenGLRendererAPI::Init()
	{
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
	}


	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

}