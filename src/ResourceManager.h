/*!
* \file ResourceManager.h
* \brief Load and store textures in a cache.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "TextureCache.h"
#include "AudioEngine.h"
#include "FontCache.h"

#include <string>

namespace NS2
{
	class ResourceManager
	{
	public:
		/*!
		* \brief Initialize all static resources.
		* \return true if no error, false else.
		*/
		static bool init();
		/*!
		* \brief Destroy all static resources.
		*/
		static void destroy();
		/*!
		* \brief Load and store sound if it isn't in cache and return it.
		* \param soundPath: sound path
		* \return sound.
		*/
		static Sound getSound(const std::string soundPath);
		/*!
		* \brief Load and store music if it isn't in cache and return it.
		* \param musicPath: music path
		* \return music.
		*/
		static Music getMusic(const std::string musicPath);

		/*!
		* \brief Load and store texture if it isn't in cache and return it.
		* \param texturePath: texture path
		* \return texture
		*/
		static GLTexture getTexture(const std::string texturePath);

		/*!
		* \brief Load and store font if it isn't in cache and return it.
		* \param fontPath: font path
		* \return font
		*/
		static SpriteFont getFont(const std::string fontPath, int size = 32);
		
	private:
		static TextureCache m_textureCache; /* Texture cache */
		static AudioEngine m_audioEngine;	/* Audio engine */
		static FontCache m_fontCache;		/* Font cache */
	};
}
