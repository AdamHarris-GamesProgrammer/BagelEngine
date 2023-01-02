#include "bgpch.h"
#include "OpenGLBuffer.h"

#include<glad/glad.h>

#include "OpenGLError.h"

namespace Bagel {
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		BG_PROFILE_FUNCTION();
		GLCall(glCreateBuffers(1, &_rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		BG_PROFILE_FUNCTION();
		GLCall(glDeleteBuffers(1, &_rendererID));
	}

	void OpenGLVertexBuffer::Bind() const
	{
		BG_PROFILE_FUNCTION();
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		BG_PROFILE_FUNCTION();
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		_layout = layout;
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return _layout;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: _count(count)
	{
		BG_PROFILE_FUNCTION();
		GLCall(glCreateBuffers(1, &_rendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		BG_PROFILE_FUNCTION();
		GLCall(glDeleteBuffers(1, &_rendererID));
	}

	void OpenGLIndexBuffer::Bind() const
	{
		BG_PROFILE_FUNCTION();
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		BG_PROFILE_FUNCTION();
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}