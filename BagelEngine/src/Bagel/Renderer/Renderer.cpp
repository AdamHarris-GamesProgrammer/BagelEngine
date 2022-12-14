#include "bgpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Bagel {
	Scope<Renderer::SceneData> Renderer::_sSceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		BG_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		_sSceneData->ViewProjection = camera.ViewProj();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<class Shader>& shader, const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform, const glm::vec4& color)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", _sSceneData->ViewProjection);
		shader->UploadUniformMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}