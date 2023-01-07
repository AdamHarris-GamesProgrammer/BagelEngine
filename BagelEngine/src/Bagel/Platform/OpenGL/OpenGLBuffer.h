#pragma once
#include "Bagel/Renderer/Buffer.h"

namespace Bagel {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer() override;

		// Inherited via VertexBuffer
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& GetLayout() const override;

	private:
		uint32_t _rendererID = 0;
		BufferLayout _layout;

	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		// Inherited via IndexBuffer
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const { return _count; }

	private:
		uint32_t _rendererID = 0;

		//How many indices does this index buffer have
		uint32_t _count = 0;
	};
}