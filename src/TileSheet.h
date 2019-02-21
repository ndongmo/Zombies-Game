/*!
* \file TileSheet.h
* \brief Simple Tile class.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "GLTexture.h"

#include <glm\glm.hpp>

namespace NS2
{
	class TileSheet
	{
	public:
		/*!
		* \brief Initialize tile stuffs.
		* \param texture: tile texture
		* \parm dims: tile dimensions
		* \parm invX: inverse texture X axe or not
		* \parm invY: inverse texture Y axe or not
		*/
		void init(const GLTexture& texture, const glm::ivec2& dims, bool invX = false, bool invY = false) 
		{
			this->texture = texture;
			this->dims = dims;
			if (invX) {
				this->invX = 1.0f;
				this->invZ = -1.0f;
			}
			else {
				this->invX = 0.0f;
				this->invZ = 1.0f;
			}
			if (invY) {
				this->invY = 1.0f;
				this->invW = -1.0f;
			}
			else {
				this->invY = 0.0f;
				this->invW = 1.0f;
			}
		}

		/*!
		* \brief Get vector of texture coordinates.
		* \param index: index of desired texture
		* \return vector of texture coordinates
		*/
		glm::vec4& getUV(int index)
		{
			int xTile = index % dims.x;
			int yTile = index / dims.x;
			
			uvs.x = (xTile + invX) / (float)dims.x;
			uvs.y = (yTile + invY) / (float)dims.y;
			uvs.z = invZ / dims.x;
			uvs.w = invW / dims.y;

			return uvs;
		}

		GLTexture texture;	/* Tile texture */
		glm::ivec2 dims;	/* Tile dimensions */
		glm::vec4 uvs;		/* UV data */
		float invX = 0, invY = 0, invZ = 1, invW = 1;		/* Use to inverse the texture */
	};
}

