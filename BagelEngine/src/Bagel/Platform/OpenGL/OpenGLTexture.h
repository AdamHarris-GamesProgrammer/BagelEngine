#pragma once

#include "Bagel/Renderer/Texture.h"

namespace Bagel {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		// Inherited via Texture2D
		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;

	private: 
		uint32_t _width, _height;
		uint32_t _rendererID;

		std::string _path;
	};
}