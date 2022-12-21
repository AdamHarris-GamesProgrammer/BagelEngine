#include "bgpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Bagel/Core.h"

#include "OpenGLError.h"

#include <fstream>
#include <filesystem>

namespace Bagel {
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		BG_CORE_ASSERT(false, "Unknown shader type!");

		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: _name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& shaderSrc)
	{
		std::string shaderCode = ReadFile(shaderSrc);

		std::unordered_map<GLenum, std::string> shaders = PreProcess(shaderCode);

		Compile(shaders);

		std::filesystem::path path = shaderSrc;

		_name = path.stem().string();
	}

	OpenGLShader::~OpenGLShader()
	{
		GLCall(glDeleteProgram(_rendererID));
	}

	void OpenGLShader::Bind() const
	{
		GLCall(glUseProgram(_rendererID));
	}

	void OpenGLShader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaders)
	{
		BG_CORE_ASSERT(shaders.size() <= 2, "Only two shaders are supported at this point in time");
		
		int glShaderIndex = 0;
		std::array<GLenum, 2> glShaderIDs;

		_rendererID = glCreateProgram();

		for (auto& kv : shaders) {
			GLenum type = kv.first;
			std::string& src = kv.second;

			GLuint shaderID = glCreateShader(type);

			const GLchar* source = (const GLchar*)src.c_str();
			GLCall(glShaderSource(shaderID, 1, &source, 0));

			// Compile the vertex shader
			GLCall(glCompileShader(shaderID));

			GLint isCompiled = 0;
			GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE) //Failed compilation
			{
				//Find out how long the error log is
				GLint maxLength = 0;
				GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength));

				// The maxLength includes the NULL character
				//Fills the infoLog vector with the shader error
				std::vector<GLchar> infoLog(maxLength);
				GLCall(glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]));

				// We don't need the shader anymore.
				GLCall(glDeleteShader(shaderID));

				BG_CORE_ERROR("{0}", infoLog.data());
				BG_CORE_ASSERT(false, "Vertex Shader Compilation Failure");

				return;
			}

			GLCall(glAttachShader(_rendererID, shaderID));

			glShaderIDs[glShaderIndex++] = shaderID;
		}

		// Link our program
		GLCall(glLinkProgram(_rendererID));

		GLint isLinked = 0;
		GLCall(glGetProgramiv(_rendererID, GL_LINK_STATUS, (int*)&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetProgramiv(_rendererID, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetProgramInfoLog(_rendererID, maxLength, &maxLength, &infoLog[0]));

			// We don't need the program anymore.
			GLCall(glDeleteProgram(_rendererID));

			// Don't leak shaders either.
			for (auto& id : glShaderIDs) {
				GLCall(glDeleteShader(id));
			}

			BG_CORE_ERROR("{0}", infoLog.data());
			BG_CORE_ASSERT(false, "Shader Link Failure");
			return;

		}

		for (auto& id : glShaderIDs) {
			GLCall(glDetachShader(_rendererID, id));
		}
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";

		size_t typeTokeLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			BG_CORE_ASSERT(eol != std::string::npos, "Syntax Error");

			size_t begin = pos + typeTokeLength + 1;
			std::string type = source.substr(begin, eol - begin);

			BG_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::in, std::ios::binary);

		std::string result;

		if (file) {
			file.seekg(0, std::ios::end);
			size_t size = file.tellg();

			result.resize(size);

			file.seekg(0, std::ios::beg);
			file.read(&result[0], size);

			file.close();

		}
		else {
			BG_CORE_ERROR("Shader file {0} does not exist", filepath);
		}

		return result;
	}

	void OpenGLShader::UploadUniformMat4(const std::string& uniformName, const glm::mat4& input)
	{
		GLint uniformLoc = glGetUniformLocation(_rendererID, uniformName.c_str());
		//BG_CORE_ASSERT(uniformLoc != 0, "Shader does not possess this uniform");
		GLCall(glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(input)));
	}

	void OpenGLShader::UploadUniformFloat(const std::string& uniformName, const float& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform1fv(location, 1, &input));
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& uniformName, const glm::vec2& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform2fv(location, 1, glm::value_ptr(input)));
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& uniformName, const glm::vec3& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform3fv(location, 1, glm::value_ptr(input)));
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& uniformName, const glm::vec4& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform4fv(location, 1, glm::value_ptr(input)));
	}

	void OpenGLShader::UploadUniformInt(const std::string& uniformName, const int& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform1i(location, input));
	}
}