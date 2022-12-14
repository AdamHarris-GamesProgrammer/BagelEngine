#pragma once
#include "Bagel/Renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>

namespace Bagel {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UploadUniformMat4(const std::string& uniformName, const glm::mat4& input) override;

	private:
		uint32_t _rendererID;

		// Inherited via Shader
		virtual void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& color) override;
	};
}

