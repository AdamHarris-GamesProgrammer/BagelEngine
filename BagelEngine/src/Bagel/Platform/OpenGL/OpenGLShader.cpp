#include "bgpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Bagel/Core.h"

#include "OpenGLError.h"

namespace Bagel {
    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
		// Read our shaders into the appropriate buffers

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		GLCall(glShaderSource(vertexShader, 1, &source, 0));

		// Compile the vertex shader
		GLCall(glCompileShader(vertexShader));

		GLint isCompiled = 0;
		GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE) //Failed compilation
		{
			//Find out how long the error log is
			GLint maxLength = 0;
			GLCall(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			//Fills the infoLog vector with the shader error
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]));



			// We don't need the shader anymore.
			GLCall(glDeleteShader(vertexShader));

			BG_CORE_ERROR("{0}", infoLog.data());
			BG_CORE_ASSERT(false, "Vertex Shader Compilation Failure");

			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		GLCall(glShaderSource(fragmentShader, 1, &source, 0));

		// Compile the fragment shader
		GLCall(glCompileShader(fragmentShader));

		GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]));

			// We don't need the shader anymore.
			GLCall(glDeleteShader(fragmentShader));
			// Either of them. Don't leak shaders.
			GLCall(glDeleteShader(vertexShader));

			BG_CORE_ERROR("{0}", infoLog.data());
			BG_CORE_ASSERT(false, "Fragment Shader Compilation Failure");

			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		_rendererID = glCreateProgram();

		// Attach our shaders to our program
		GLCall(glAttachShader(_rendererID, vertexShader));
		GLCall(glAttachShader(_rendererID, fragmentShader));

		// Link our program
		GLCall(glLinkProgram(_rendererID));

		// Note the different functions here: glGetProgram* instead of glGetShader*.
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
			GLCall(glDeleteShader(vertexShader));
			GLCall(glDeleteShader(fragmentShader));

			BG_CORE_ERROR("{0}", infoLog.data());
			BG_CORE_ASSERT(false, "Shader Link Failure");
			return;
		}

		// Always detach shaders after a successful link.
		GLCall(glDetachShader(_rendererID, vertexShader));
		GLCall(glDetachShader(_rendererID, fragmentShader));
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

	void OpenGLShader::UploadUniformMat4(const std::string& uniformName, const glm::mat4& input)
	{
		GLint uniformLoc = glGetUniformLocation(_rendererID, uniformName.c_str());
		//BG_CORE_ASSERT(uniformLoc != 0, "Shader does not possess this uniform");
		GLCall(glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(input)));
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& uniformName, const glm::vec4& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform4fv(location, 1, glm::value_ptr(input)));
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

	void OpenGLShader::UploadUniformInt(const std::string& uniformName, const int& input)
	{
		GLint location = glGetUniformLocation(_rendererID, uniformName.c_str());
		GLCall(glUniform1i(location, input));
	}
}