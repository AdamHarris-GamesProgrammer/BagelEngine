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

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		//Getters
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
			return _pVertexBuffers;
		}
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override {
			return _pIndexBuffer;
		}

	private:
		//Vertex Array ID
		uint32_t _rendererID = 0;

		//A Vertex Array can hold multiple vertex buffers
		std::vector<Ref<VertexBuffer>> _pVertexBuffers;

		//A vertex array can only hold one index array
		Ref<IndexBuffer> _pIndexBuffer;
	};
}
