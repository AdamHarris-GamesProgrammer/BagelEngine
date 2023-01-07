#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Bagel {
	class BG_API RendererAPI {
	public:
		//Used for controlling what RendererAPI we are currently using. NOTE: Will be expanded in future
		enum class API {
			None = 0,
			OpenGL = 1,
			Direct3D = 2
		};

	public:
		virtual void Init() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		inline static API GetAPI() { return _sAPI; }

	private:
		static RendererAPI::API _sAPI;
	};

}

