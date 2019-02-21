#include "FontCache.h"
#include <string>

namespace NS2
{
	SpriteFont FontCache::getFont(const std::string fontPath, int size)
	{
		auto mit = m_fontMap.find(fontPath);

		// check if it is not in the map
		if (mit == m_fontMap.end()) {
			// load the texture
			SpriteFont font;
			font.init(fontPath.c_str(), size);

			// insert loaded texture in the map
			m_fontMap.insert(make_pair(fontPath + std::to_string(size), font));

			return font;
		}

		return mit->second;
	}

	void FontCache::destroy()
	{
		for (auto& p : m_fontMap){
			p.second.dispose();
		}
	}
}