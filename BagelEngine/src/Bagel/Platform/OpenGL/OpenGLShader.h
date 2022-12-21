#pragma once
#include "Bagel/Renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Bagel {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& shaderSrc);

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override {
			return _name;
		}

		virtual void UploadUniformFloat(const std::string& uniformName, const float& input) override;
		virtual void UploadUniformFloat2(const std::string& uniformName, const glm::vec2& input) override;
		virtual void UploadUniformFloat3(const std::string& uniformName, const glm::vec3& input) override;
		virtual void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& input) override;

		virtual void UploadUniformInt(const std::string& uniformName, const int& input) override;
		virtual void UploadUniformMat4(const std::string& uniformName, const glm::mat4& input) override;

	private:
		void Compile(std::unordered_map<GLenum, std::string> shaders);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		std::string ReadFile(const std::string& filepath);

	private:
		uint32_t _rendererID;
		std::string _name;
	};
}

