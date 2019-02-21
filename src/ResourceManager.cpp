#include "ResourceManager.h"

namespace NS2
{
	TextureCache ResourceManager::m_textureCache;
	AudioEngine ResourceManager::m_audioEngine;
	FontCache ResourceManager::m_fontCache;

	bool ResourceManager::init()
	{
		return m_audioEngine.init();
	}

	void ResourceManager::destroy()
	{
		m_audioEngine.destroy();
		m_fontCache.destroy();
	}

	Sound ResourceManager::getSound(const std::string soundPath)
	{
		return m_audioEngine.loadSound(soundPath);
	}

	Music ResourceManager::getMusic(const std::string musicPath)
	{
		return m_audioEngine.loadMusic(musicPath);
	}

	GLTexture ResourceManager::getTexture(const std::string texturePath) {
		return m_textureCache.getTexture(texturePath);
	}

	SpriteFont ResourceManager::getFont(const std::string fontPath, int size/* = 32*/)
	{
		return m_fontCache.getFont(fontPath, size);
	}
}