#include "TextureCache.h"
#include "ImageLoader.h"

namespace NS2 
{
	GLTexture TextureCache::getTexture(const std::string texturePath)
	{
		auto mit = m_textureMap.find(texturePath);

		// check if it is not in the map
		if (mit == m_textureMap.end()) {
			// load the texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			if (newTexture.height > 0 && newTexture.width > 0)
			{
				// insert loaded texture in the map
				m_textureMap.insert(make_pair(texturePath, newTexture));
			}

			return newTexture;
		}

		return mit->second;
	}
}