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

	void Renderer::Submit(const std::shared_ptr<class Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", _sSceneData->ViewProjection);
		shader->UploadUniformMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}