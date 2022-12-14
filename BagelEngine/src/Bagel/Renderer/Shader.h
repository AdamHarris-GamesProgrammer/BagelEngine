#pragma once

#include <glm/glm.hpp>

namespace Bagel {
	//Abstract class for representing a Shader across different APIs
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& color) = 0;
		virtual void UploadUniformMat4(const std::string& uniformName, const glm::mat4& input) = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	};
}
