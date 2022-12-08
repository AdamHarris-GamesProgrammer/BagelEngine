#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Bagel {

	class Renderer {
	public:
		static void BeginScene(const OrthographicCamera& camera); //TODO Scene Params
		static void EndScene();

		static void Submit(const std::shared_ptr<class Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjection;
		};

		static SceneData* _sSceneData;
	};

}
