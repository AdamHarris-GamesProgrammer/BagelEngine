#include "bgpch.h"
#include "Renderer.h"

namespace Bagel {
	Renderer::SceneData* Renderer::_sSceneData = new Renderer::SceneData;

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
		shader->UploadUniformFloat4("u_Color", color);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}