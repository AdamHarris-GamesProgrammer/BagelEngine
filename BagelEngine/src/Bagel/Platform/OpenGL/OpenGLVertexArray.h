#pragma once
#include "Bagel/Renderer/VertexArray.h"

namespace Bagel {
	//OpenGL Specific Vertex Array
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		//Getters
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
			return _pVertexBuffers;
		}
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {
			return _pIndexBuffer;
		}

	private:
		//Vertex Array ID
		uint32_t _rendererID = 0;

		//A Vertex Array can hold multiple vertex buffers
		std::vector<std::shared_ptr<VertexBuffer>> _pVertexBuffers;

		//A vertex array can only hold one index array
		std::shared_ptr<IndexBuffer> _pIndexBuffer;
	};
}
