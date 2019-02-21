#include "AudioEngine.h"
#include "Logger.h"

namespace NS2 {

#define ALLOCATE_CHANNELS 16

	int Sound::m_lastChannel = 0;

	void Sound::play(int loops) 
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) 
		{
			if (Mix_PlayChannel(0, m_chunk, loops) == -1) 
			{
				logE << "Mix_PlayChannel : " << std::string(Mix_GetError()) << std::endl;
			}
		}
	}

	void Sound::play(int loops, Uint8 distance)
	{
		
		if (Mix_SetDistance(m_lastChannel, distance)) {
			if ((m_lastChannel = Mix_PlayChannel(m_lastChannel, m_chunk, loops)) == -1)
			{
				if ((m_lastChannel = Mix_PlayChannel(0, m_chunk, loops)) == -1)
				{
					logE << "Mix_PlayChannel : " << std::string(Mix_GetError()) << std::endl;
				}
			}
		}
		else
		{
			Mix_SetDistance(MIX_CHANNEL_POST, distance);

			if ((m_lastChannel = Mix_PlayChannel(-1, m_chunk, loops)) == -1)
			{
				if ((m_lastChannel = Mix_PlayChannel(0, m_chunk, loops)) == -1)
				{
					logE << "Mix_PlayChannel : " << std::string(Mix_GetError()) << std::endl;
				}
			}
		}
		//logI << Mix_AllocateChannels(-1) << " last " << m_lastChannel << " dist " << (int)distance << std::endl;
		m_lastChannel++;
		if (m_lastChannel >= Mix_AllocateChannels(-1))
			m_lastChannel = 0;
	}

	void Music::play(int loops) 
	{
		if (Mix_PlayMusic(m_music, loops) == -1) 
		{
			logE << "Mix_PlayMusic : " << std::string(Mix_GetError()) << std::endl;
		}
	}

	int Music::playing()
	{
		return Mix_PlayingMusic();
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	bool AudioEngine::init()
	{
		if(m_initialized) 
		{
			logW << "Tried to initialize Audio engine twice" << std::endl;
			return true;
		}

		// Parameter can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MP3, MIX_INIT_OGG and so on
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 1) 
		{
			logE << "MIX_INIT : " << std::string(Mix_GetError()) << std::endl;
			return false;
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)) 
		{
			logE << "MIX_INIT : " << std::string(Mix_GetError()) << std::endl;
			return false;
		}

		Mix_AllocateChannels(ALLOCATE_CHANNELS);

		return m_initialized = true;
	}

	void AudioEngine::destroy()
	{
		if (m_initialized) 
		{
			for (auto& it : m_effectMap)  {
				Mix_FreeChunk(it.second);
			}

			for (auto& it : m_musicMap)  {
				Mix_FreeMusic(it.second);
			}

			m_initialized = false;
			m_effectMap.clear();
			m_musicMap.clear();
			Mix_Quit();
		}
	}

	Sound AudioEngine::loadSound(const std::string& filePath)
	{
		Sound effect;

		auto it = m_effectMap.find(filePath);

		if (it == m_effectMap.end()) 
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			
			if (chunk == nullptr) { 
				logE << "Mix_LoadWAV : " << std::string(Mix_GetError()) << std::endl;
			}
			else {
				effect.m_chunk = chunk;
				effect.m_filePath = filePath;
				m_effectMap[filePath] = chunk;
			}
		} 
		else 
		{
			effect.m_chunk = it->second;
			effect.m_filePath = filePath;
		}

		return effect;
	}

	Music AudioEngine::loadMusic(const std::string& filePath)
	{
		Music music;

		auto it = m_musicMap.find(filePath);

		if (it == m_musicMap.end()) 
		{
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());

			if (mixMusic == nullptr) {
				logE << "Mix_LoadMUS : " << std::string(Mix_GetError()) << std::endl;
			}
			else {
				music.m_music = mixMusic;
				music.m_filePath = filePath;
				m_musicMap[filePath] = mixMusic;
			}
		} 
		else
		{
			music.m_music = it->second;
		}

		return music;
	}

}