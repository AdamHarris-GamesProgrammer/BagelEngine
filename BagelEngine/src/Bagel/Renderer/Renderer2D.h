#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Bagel {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//Primitives
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
		
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
	};
}