#include "IMainGame.h"
#include "IGameScreen.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include "Logger.h"

#include <SDL\SDL.h>

namespace NS2
{
	bool IMainGame::init(std::string windowName, int screenWidth, int screenHeight,
		unsigned int currentFlags)
	{

#ifndef _DEBUG
		FILE *stream;
		freopen_s(&stream, "log.log","w",stdout);
#endif
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		bool state = m_window.create(windowName, screenWidth, screenHeight, currentFlags);
		if (!state) return state;

		state = ResourceManager::init();
		if (!state) return state;

		m_eventHandler.init(screenWidth, screenHeight);

		state = m_gui.init("gui");
		if (!state) return state;

		m_screenManager.init(this);

		onInit();

		addScreens();

		m_currentScreen = m_screenManager.getCurrentScreen();
		m_currentScreen->onEntry();
		m_currentScreen->setGameState(ScreenState::RUNNING);

		return true;
	}

	void IMainGame::onSDLEvent(SDL_Event& ev)
	{
		if (m_eventHandler.getPlayWith() == PlayWith::KEYBOARD)
		{
			switch (ev.type) {
			case SDL_QUIT:
				m_currentScreen->setGameState(NS2::ScreenState::CHANGE_PREVIOUS);
				break;
			case SDL_MOUSEMOTION:
				m_eventHandler.setMouseCoords((float)ev.motion.x, (float)ev.motion.y);
				break;
			case SDL_KEYDOWN:
				m_eventHandler.pressKey(ev.key.keysym.sym);
				break;
			case SDL_KEYUP:
				m_eventHandler.releaseKey(ev.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_eventHandler.pressKey(ev.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				m_eventHandler.releaseKey(ev.button.button);
				break;
			}
		}
		else
		{
			switch (ev.type) {
			case SDL_QUIT:
				m_currentScreen->setGameState(NS2::ScreenState::CHANGE_PREVIOUS);
				break;
			case SDL_JOYBUTTONDOWN:
				m_eventHandler.pressKey(ev.jbutton.button + MARGIN);
				break;
			case SDL_JOYBUTTONUP:
				m_eventHandler.releaseKey(ev.jbutton.button + MARGIN);
				break;
			case SDL_JOYAXISMOTION:
				m_eventHandler.updateJoystickAxis(ev.jaxis.axis, ev.jaxis.value, m_window.getSDLWindow());
				break;
			case SDL_JOYHATMOTION:
				m_eventHandler.updateJoystickHats(ev.jhat.value + MARGIN);
				break;
			}
		}

		m_gui.onSDLEvent(ev);
	}

	void IMainGame::update()
	{
		if (m_currentScreen)
		{
			switch (m_currentScreen->getGameState())
			{
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenManager.moveNext();
				if (m_currentScreen)
				{
					m_currentScreen->setGameState(ScreenState::RUNNING);
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenManager.movePrevious();
				if (m_currentScreen)
				{
					m_currentScreen->onEntry();
					m_currentScreen->setGameState(ScreenState::RUNNING);
				}
				break;
			case ScreenState::EXIT_GAME:
				m_isRunning = false;
				break;
			default:
				break;
			}
		}
		else
		{
			m_isRunning = false;
		}
	}

	void IMainGame::draw()
	{
		if (m_currentScreen && m_currentScreen->getGameState() == ScreenState::RUNNING)
		{
			glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
			m_currentScreen->draw();
			m_gui.draw();
		}
	}

	void IMainGame::run(std::string windowName, int screenWidth, int screenHeight, 
		unsigned int currentFlags)
	{
		m_isRunning = init(windowName, screenWidth, screenHeight, currentFlags);

		m_limiter.setMaxFPS(MAX_FPS);

		while (m_isRunning)
		{
			m_limiter.begin();
			{
				m_eventHandler.update(m_limiter.getFPS());

				update();

				draw();
			}
			m_limiter.end();
			m_window.swapBuffer();
		}

		exit();
	}

	void IMainGame::exit()
	{
		m_currentScreen->onExit();
		m_screenManager.destroy();
		m_eventHandler.destroy();
		ResourceManager::destroy();
		m_gui.destroy();
		SDL_Quit();
	}
}
