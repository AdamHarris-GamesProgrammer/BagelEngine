#pragma once

#include "Bagel/Renderer/RendererAPI.h"

namespace Bagel {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		// Inherited via RendererAPI
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};
}
