#pragma once
#include "Bagel/Renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>

namespace Bagel {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;


		virtual void UploadUniformFloat(const std::string& uniformName, const float& input) override;
		virtual void UploadUniformFloat2(const std::string& uniformName, const glm::vec2& input) override;
		virtual void UploadUniformFloat3(const std::string& uniformName, const glm::vec3& input) override;
		virtual void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& input) override;

		virtual void UploadUniformInt(const std::string& uniformName, const int& input) override;
		virtual void UploadUniformMat4(const std::string& uniformName, const glm::mat4& input) override;

	private:
		uint32_t _rendererID;

	};
}

