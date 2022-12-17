#pragma once

#include <glm/glm.hpp>

namespace Bagel {
	//Abstract class for representing a Shader across different APIs
	class Shader {
	public:
		virtual ~Shader() = default;

		//Bind/Unbind functions
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		//Handles creating a platform specific shader
		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		//UNIFORMS
		virtual void UploadUniformMat4(const std::string& uniformName, const glm::mat4& input) = 0;

		virtual void UploadUniformFloat(const std::string& uniformName, const float& input) = 0;
		virtual void UploadUniformFloat2(const std::string& uniformName, const glm::vec2& input) = 0;
		virtual void UploadUniformFloat3(const std::string& uniformName, const glm::vec3& input) = 0;
		virtual void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& input) = 0;

		virtual void UploadUniformInt(const std::string& uniformName, const int& input) = 0;

	};
}
