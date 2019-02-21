#include "GameWindow.h"
#include "Logger.h"

namespace NS2
{
	bool GameWindow::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE){
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN){
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS){
			flags |= SDL_WINDOW_BORDERLESS;
		}

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (m_window == nullptr) {
			logE << "SDL window could not be created !" << std::endl;
			return false;
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
		if (glContext == nullptr) {
			logE << "SDL_GL context could not be created!" << std::endl;
			return false;
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			logE << "Could not initialize GLEW!" << std::endl;
			return false;
		}

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		SDL_GL_SetSwapInterval(0);

		glClearDepth(1.0);

		// enable alpha blend
		glEnable(GL_BLEND);

		// it tells to GL how to blend. First parameter specify that it will multiply color with source alpha for obtain draw color. 
		// so, if alpha equals 0 there will be nothing to draw, else if alpha equals 1 the whole color will be draw.
		// Second parameter is the destination factor. it allows to draw clear color, if source alpha equals 1 no need to draw clear color.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SDL_GetWindowSize(m_window, &m_screenWidth, &m_screenHeight);
		return true;
	}

	void GameWindow::swapBuffer() {
		SDL_GL_SwapWindow(m_window);
	}
}
