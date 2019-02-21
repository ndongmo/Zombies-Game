/*!
* \file AudioEngine.h
* \brief Simple Sound and Music manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <string>
#include <map>
#include <SDL\SDL_mixer.h>

namespace NS2 {
	/*!
	* \brief Sound class.
	*/
	class Sound {
	public:
		friend class AudioEngine;

		/*!
		* \brief Play a sound.
		* \parm loop : loop the sound that many times (See SDL_MIXER doc)
		*/
		void play(int loops = 0);
		void play(int loops, Uint8 distance);

		/* GETTERS */
		const std::string& getFilePath() const { return m_filePath; }

	private:
		static int m_lastChannel;	/* Last playing channel */
		Mix_Chunk* m_chunk;		/* Audio chunk */
		std::string m_filePath;	/* Audio file name */
	};

	class Music {
	public:
		friend class AudioEngine;
		/*!
		* \brief Play an audio.
		* \parm loop : loop the sound that many times (See SDL_MIXER doc)
		*/
		void play(int loops = -1);

		/*!
		* \brief Check if music is playing.
		*/
		static int playing();
		/*!
		* \brief Pause currently playing audio.
		*/
		static void pause();
		/*!
		* \brief Resume currently playing audio.
		*/
		static void resume();
		/*!
		* \brief Stop currently playing audio.
		*/
		static void stop();

		/* GETTERS */
		const std::string& getFilePath() const { return m_filePath; }

	private:
		Mix_Music* m_music;		/* Music chunk */
		std::string m_filePath;	/* Music file name */
	};

	/*!
	* \brief Audio class manager.
	*/
	class AudioEngine
	{
	public:
		/*!
		* \brief Initialize all audio stuffs.
		*/
		bool init();
		/*!
		* \brief Free all audio resources.
		*/
		void destroy();
		/*!
		* \brief Load a sound file.
		* \param filePath: sound file path
		* \return Sound object
		*/
		Sound loadSound(const std::string& filePath);
		/*!
		* \brief Load a music file.
		* \param filePath: music file path
		* \return Music object
		*/
		Music loadMusic(const std::string& filePath);

	private:
		bool m_initialized;								/* Keep audio state */

		std::map<std::string, Mix_Chunk*> m_effectMap;	/* Map of audio chuncks */
		std::map<std::string, Mix_Music*> m_musicMap;	/* Map of music chuncks */
	};

}