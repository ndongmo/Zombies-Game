/*!
* \file GLTexture.h
* \brief Simple OpenGL 2D Texture structure.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <string>
#include <GL/glew.h>

namespace NS2
{
	struct GLTexture 
	{
		std::string filePath;
		GLuint id;
		int width;
		int height;
	};
}