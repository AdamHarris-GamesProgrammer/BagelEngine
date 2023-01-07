#pragma once

#include "Bagel/Renderer/Texture.h"
#include <glad/glad.h>

namespace Bagel {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);


		virtual void SetData(void* data, size_t size) override;

		virtual ~OpenGLTexture2D();

		// Inherited via Texture2D
		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;

	private: 
		uint32_t _width, _height;
		uint32_t _rendererID;

		GLenum _internalFormat, _dataFormat;

		std::string _path;

		// Inherited via Texture2D
		virtual bool operator==(const Texture2D& other) const override;
	};
}