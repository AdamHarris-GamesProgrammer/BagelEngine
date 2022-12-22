#pragma once
#include "Bagel/Renderer/Shader.h"

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

//TODO remove this include and take shader data type into it's own header
#include <Bagel/Renderer/Buffer.h>

#include <glad/glad.h>

namespace Bagel {
	struct UniformData {
		ShaderDataType type;
		GLuint location;
	};

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

		UniformData GetUniform(const std::string& uniformName);

		bool DoesUniformExist(const std::string& unformName) const;

	private:
		void Compile(std::unordered_map<GLenum, std::string> shaders);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		std::unordered_map<std::string, UniformData> FindUniforms(const std::string& source);
		std::string ReadFile(const std::string& filepath);

	private:
		std::unordered_map<std::string, UniformData> _uniforms;

		uint32_t _rendererID;
		std::string _name;
	};
}

