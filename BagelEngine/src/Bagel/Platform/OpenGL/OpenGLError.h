#pragma once

//OpenGL Stuff
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

#include <string>
#include <iostream>

#include "Bagel/Core/Core.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef BG_DEBUG
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#endif // DEBUG

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR) {

	}
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::string errorString = "";
		switch (error) {
		case GL_INVALID_ENUM:
			errorString = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_OPERATION:
			errorString = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_INDEX:
			errorString = "GL_INVALID_INDEX";
			break;
		case GL_INVALID_VALUE:
			errorString = "GL_INVALID_ERROR";
			break;
		}

		BG_CORE_ERROR("[OpenGL Error]: ({0}) {1} {2}: {3}", errorString, function, file, line);

		return false;
	}
	return true;
}