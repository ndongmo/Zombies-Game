/*!
* \file FontCache.h
* \brief Font cache manager.
* \author Ndongmo Silatsa Fabrice
* \date 24-02-2017
* \version 1.0
*/

#pragma once

#include "SpriteFont.h"

#include <map>

namespace NS2 {

	class FontCache
	{
	public:
		/*!
		* \brief Dispose every font.
		*/
		void destroy();

		/*!
		* \brief Get Font from font map if this font is already loaded.
		*			Otherwise, load font in font map and return it.
		* \param fontPath: Font file path
		*/
		SpriteFont getFont(const std::string fontPath, int size);

	private:
		std::map<std::string, SpriteFont> m_fontMap; /* Font map */
	};
}
