#include "bgpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Bagel/Platform/OpenGL/OpenGLShader.h"

namespace Bagel {

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI Selected!");

		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& shaderSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:		BG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLShader>(shaderSrc);
		}

		BG_CORE_ASSERT(false, "Unknown RendererAPI Selected!");

		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) 
	{
		const std::string& name = shader->GetName();
		BG_CORE_ASSERT(!Exists(name), "Shader already exists");
		_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		BG_CORE_ASSERT(!Exists(name), "Shader already exists");
		_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) 
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vs, const std::string& fs)
	{
		Ref<Shader> shader = Shader::Create(name, vs, fs);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		BG_CORE_ASSERT(Exists(name), "Couldn't find shader with name: {0}", name);
		return _shaders[name];
	}

	void ShaderLibrary::Remove(const std::string& shaderName)
	{
		if (Exists(shaderName)) {
			_shaders.erase(shaderName);
		}
	}

	bool ShaderLibrary::Exists(const std::string& name) {
		return _shaders.find(name) != _shaders.end();
	}
}