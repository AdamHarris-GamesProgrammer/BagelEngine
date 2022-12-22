#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Bagel/Core.h"

namespace Bagel {
	//Abstract class for representing a Shader across different APIs
	class Shader {
	public:
		virtual ~Shader() = default;

		//Bind/Unbind functions
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		//Handles creating a platform specific shader
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& shaderSrc);

		virtual const std::string& GetName() const = 0;

		//UNIFORMS
		virtual void UploadUniformMat4(const std::string& uniformName, const glm::mat4& input) = 0;

		virtual void UploadUniformFloat(const std::string& uniformName, const float& input) = 0;
		virtual void UploadUniformFloat2(const std::string& uniformName, const glm::vec2& input) = 0;
		virtual void UploadUniformFloat3(const std::string& uniformName, const glm::vec3& input) = 0;
		virtual void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& input) = 0;

		virtual void UploadUniformInt(const std::string& uniformName, const int& input) = 0;
	};


	class ShaderLibrary {
	public:

		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& vs, const std::string& fs);

		Ref<Shader> Get(const std::string& name);

		void Remove(const std::string& name);

		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> _shaders;
	};
}
