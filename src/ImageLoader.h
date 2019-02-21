/*!
* \file ImageLoader.h
* \brief Load PNG image to GLTexture.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "GLTexture.h"

#include <string>

namespace NS2 
{
	class ImageLoader
	{
	public:
		/*!
		* \brief create GLTexture.
		*/
		static GLTexture create(GLsizei width, GLsizei height);
		/*!
		* \brief Load PNG image to GLTexture.
		* \param filePath: PNG file path
		*/
		static GLTexture loadPNG(const std::string filePath);
		/*!
		* \brief create PNG image from pixels.
		* \param filePath: PNG file path
		* \param x: x coordinate of origin pixel
		* \param y: y coordinate of origin pixel
		* \param width: image width
		* \param height: image height
		*/
		static void savePNG(
			GLint x,
			GLint y,
			GLsizei width,
			GLsizei height,
			const std::string filePath);
	};
}
