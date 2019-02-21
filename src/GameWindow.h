/*!
* \file GameWindow.h
* \brief Main game window (SDL_Window).
* \author Ndongmo Silatsa Fabrice
* \date 24-10-2016
* \version 1.0
*/

#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace NS2
{
	/*!
	* \brief Different states of window.
	*/
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class GameWindow
	{
	public:
		/*!
		* \brief Create the window.
		* \paaram windowName: window name
		* \param screenWidth: window width
		* \param screenHeight: window height
		* \param currentFlags: window flags for configuration
		*/
		bool create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		/*!
		* \brief Swap the OpenGL buffers for window, if double-buffering is supported.
		*/
		void swapBuffer();

		/* GETTERS */
		int getScreenWidth()  { return m_screenWidth; }
		int getScreenHeight() { return m_screenHeight; }
		SDL_Window* getSDLWindow() const { return m_window; }

	private:
		SDL_Window* m_window;	/* Pointer to SDL_Window */
		int m_screenWidth;		/* Screen width */
		int m_screenHeight;		/* Screen height */
	};
}