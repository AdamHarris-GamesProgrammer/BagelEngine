#include "bgpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "stb_image.h"

#include "OpenGLError.h"

namespace Bagel {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: _rendererID(0), _path(path)
	{
		BG_PROFILE_FUNCTION();

		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;
		stbi_uc* data;
		{
			BG_PROFILE_SCOPE("OpenGLTexture2D Loading texture")
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			BG_CORE_ASSERT(data, "Failed to load image!");
		}

		_width = width;
		_height = height;

		GLenum dataFormat = 0;
		GLenum internalFormat = 0;

		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		_internalFormat = internalFormat;
		_dataFormat = dataFormat;

		BG_CORE_ASSERT(internalFormat && dataFormat, "Trying to load unsupported format!");

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &_rendererID));

		GLCall(glTextureStorage2D(_rendererID, 1, internalFormat, _width, _height));

		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data));

		//Deallocate memory assocaited with the image
		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: _width(width), _height(height)
	{
		BG_PROFILE_FUNCTION();

		GLenum internalFormat = GL_RGBA8;
		GLenum dataFormat = GL_RGBA;

		_internalFormat = internalFormat;
		_dataFormat = dataFormat;
		
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &_rendererID));

		GLCall(glTextureStorage2D(_rendererID, 1, internalFormat, _width, _height));

		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));


	}

	void OpenGLTexture2D::SetData(void* data, size_t size)
	{
		BG_PROFILE_FUNCTION();

		uint32_t bytes = _dataFormat == GL_RGBA ? 4 : 3;
		BG_CORE_ASSERT(size == _width * _height * bytes, "Data must be entire texture!");
		GLCall(glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data));
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		BG_PROFILE_FUNCTION();

		GLCall(glDeleteTextures(1, &_rendererID));
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		BG_PROFILE_RENDERER_FUNCTION();
		GLCall(glBindTextureUnit(slot, _rendererID));
	}
	void OpenGLTexture2D::Unbind() const
	{
		BG_PROFILE_RENDERER_FUNCTION();
		GLCall(glBindTextureUnit(0, 0));
	}
}