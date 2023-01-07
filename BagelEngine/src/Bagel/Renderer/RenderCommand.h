#pragma once

#include "RendererAPI.h"

namespace Bagel {
	class RenderCommand {
	public:
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			_sRendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void Init() {
			_sRendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			_sRendererAPI->SetViewport(x, y, width, height);
		}


		inline static void SetClearColor(const glm::vec4& color) {
			_sRendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			_sRendererAPI->Clear();
		}

		 

	private:
		static Scope<RendererAPI> _sRendererAPI;
	};
}
