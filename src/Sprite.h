/*!
* \file Sprite.h
* \brief 2D Sprite.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <GL\glew.h>
#include <string>

#include "GLTexture.h"

namespace NS2
{

	class Sprite
	{
	public:
		Sprite(void);
		~Sprite(void);
		/*!
		* \brief Initialize Sprite stuffs.
		*/
		void init(float x, float y, float width, float height, const std::string texturePath);
		/*!
		* \brief Draw Sprite.
		*/
		void draw();

	private:
		float m_x;				/* X coordinate */
		float m_y;				/* Y coordinate */
		float m_width;			/* Width */
		float m_height;			/* Height */
		GLuint m_vboID;			/* ID of sprite in vertex buffer object */
		GLTexture m_texture;	/* Sprite texture */
	};
}
