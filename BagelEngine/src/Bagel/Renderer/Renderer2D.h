#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "Bagel/Math/Transform.h"

namespace Bagel {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		/* Improved RendererAPI Idea
		
		- Objects will not generate a transform matrix every frame, instead they will only generate the transform mat when their position, size, or rotation changes. 
		  This will reduce the amount of matrices operations per frame
		- Still need to implement a proper gameobject component system where every object has some form of transform.


		struct Transform {
			glm::mat4 transformMat = glm::mat4(1.0f);
			glm::vec3 pos = glm::vec3(1.0f);
			glm::vec2 scale = glm::vec2(1.0f);
			float rot = 0.0f; //radians
		}

		- In the future the plan will be for the renderer to sort all objects before rendering them in order of shader usage, texture usage etc to minimise state changing on gpu.
		  When this is the case a Ref to the texture will not be required in the DrawInfo struct as the pre render stage will figure out the texture index and bind that slot when it
		  is time to draw the quad

		struct DrawInfo {
			//int textureIndex = 0 (0 is white texture) 
			Ref<Texture2D> texture = nullptr;
			glm::vec4 color = glm::vec4(1.0f);
			float textureScale = 1.0f;
		}

		Transform& transform = obj->GetTransform();
		DrawInfo& drawInfo = obj->GetDrawInfo();

		Renderer2D::DrawQuad(transform, drawInfo);
		
		*/

		//Primitives
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const Transform& trans, const glm::vec4& color = glm::vec4(1.0f));
		
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float& textureScaling = 1.0f);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float& textureScaling = 1.0f);
		static void DrawQuad(const Transform& trans, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float& textureScaling = 1.0f);
	};
}