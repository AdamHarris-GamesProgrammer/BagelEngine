#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Bagel {

	class Renderer {
	public:
		static void Init();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera); //TODO Scene Params
		static void EndScene();

		static void Submit(const std::shared_ptr<class Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f), const glm::vec4& color = glm::vec4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjection;
		};

		static SceneData* _sSceneData;
	};

}
