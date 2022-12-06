#pragma once

#include <memory>
#include <vector>

#include "Bagel/Renderer/Buffer.h"

namespace Bagel {
	//Abstract class for representing a VertexArray across different APIs
	class VertexArray 
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		//Getters
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		//Handles creation of a VertexArray based on the desired API
		static VertexArray* Create();
	};
}
