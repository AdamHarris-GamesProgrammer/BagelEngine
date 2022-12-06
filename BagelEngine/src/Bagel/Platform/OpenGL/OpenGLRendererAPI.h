#pragma once

#include "Bagel/Renderer/RendererAPI.h"

namespace Bagel {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		// Inherited via RendererAPI
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};
}
