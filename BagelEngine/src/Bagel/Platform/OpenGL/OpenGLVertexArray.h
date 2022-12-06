#pragma once
#include "Bagel/Renderer/VertexArray.h"

namespace Bagel {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		// Inherited via VertexArray
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
			return _pVertexBuffers;
		}
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {
			return _pIndexBuffer;
		}

	private:
		uint32_t _rendererID = 0;
		std::vector<std::shared_ptr<VertexBuffer>> _pVertexBuffers;
		std::shared_ptr<IndexBuffer> _pIndexBuffer;
	};
}
