/*!
* \file TextureCache.h
* \brief Texture cache manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "GLTexture.h"

#include <map>

namespace NS2 {

	class TextureCache
	{
	public:
		/*!
		* \brief Get Texture from texture map if this texture is already loaded.
		*			Otherwise, load texture in texture map and return it.
		* \param texturePath: PNG file path
		*/
		GLTexture getTexture(const std::string texturePath);

	private:
		std::map<std::string, GLTexture> m_textureMap; /* Texture map */
	};
}
