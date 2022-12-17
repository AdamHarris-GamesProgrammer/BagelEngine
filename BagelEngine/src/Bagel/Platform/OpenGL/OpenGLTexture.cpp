#include "bgpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "stb_image.h"

#include "OpenGLError.h"

namespace Bagel {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: _rendererID(0), _path(path)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		BG_CORE_ASSERT(data, "Failed to load image!");

		_width = width;
		_height = height;

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &_rendererID));
		GLCall(glTextureStorage2D(_rendererID, 1, GL_RGB8, _width, _height));

		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, data));

		//Deallocate memory assocaited with the image
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCall(glDeleteTextures(1, &_rendererID));
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		GLCall(glBindTextureUnit(slot, _rendererID));
	}
}