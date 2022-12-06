#include "bgpch.h"
#include "OpenGLVertexArray.h"

#include<glad/glad.h>

namespace Bagel {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::None:		return 0;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}

		BG_CORE_ASSERT(false, "Unknown ShaderDataType!");

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &_rendererID);
		glBindVertexArray(_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &_rendererID);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		BG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(_rendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (auto& elem : layout) {
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(index, elem.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(elem.Type),
				elem.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)elem.Offset);

			index++;
		}

		_pVertexBuffers.emplace_back(vertexBuffer);

	}

	inline void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		glBindVertexArray(_rendererID);
		indexBuffer->Bind();

		_pIndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(_rendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}
