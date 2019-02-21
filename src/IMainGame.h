/*!
* \file IMainGame.h
* \brief This class represent the main game class. 
*        It contains all game stuffs and runs game main process.
* \author Ndongmo Silatsa Fabrice
* \date 24-10-2016
* \version 1.0
*/

#pragma once

#include "FPSLimiter.h"
#include "GameWindow.h"
#include "EventHandler.h"
#include "ScreenManager.h"
#include "AudioEngine.h"
#include "Gui.h"

#include <SDL/SDL.h>

#include <string>
#include <memory>

namespace NS2
{

#define MAX_FPS 60.0f
#define MARGIN 500 /* Avoid intersection between keyboard ID and gamepad ID */

	class IGameScreen;

	class IMainGame
	{
	public:
		/*!
		* \brief Run the main game procss.
		* \paaram windowName: window name
		* \param screenWidth: window width
		* \param screenHeight: window height
		* \param currentFlags: window flags for configuration
		*/
		void run(std::string windowName, int screenWidth, int screenHeight, 
			unsigned int currentFlags = 0);
		/*!
		* \brief exit game method.
		*/
		void exit();
		/*!
		* \brief Run the main game procss.
		* \param ev: SDL event
		*/
		void onSDLEvent(SDL_Event& ev);
		/*!
		* \brief Custom initialization method for IMainGame children classes.
		*/
		virtual void onInit() = 0;
		/*!
		* \brief Custom exit game method for IMainGame children classes.
		*/
		virtual void onExit() = 0;
		/*!
		* \brief Add game screens to screens manager.
		*/
		virtual void addScreens() = 0;

		/* SETTERS */
		void setMaxFPS(float max){ m_limiter.setMaxFPS(max); }

		/* GETTERS */
		EventHandler& getEventHandler() { return m_eventHandler; }
		GameWindow& getWindow() { return m_window; }
		Gui& getGui() { return m_gui; }
		float getFps() const { return m_limiter.getFPS(); }


	protected:
		/*!
		* \brief Update game dynamic stuffs and refresh game fps.
		*/
		virtual void update();
		/*!
		* \brief Draw game stuffs.
		*/
		virtual void draw();
		/*!
		* \brief Initialize all game objects and configurations.
		*/
		bool init(std::string windowName, int screenWidth, int screenHeight, 
			unsigned int currentFlags);

		ScreenManager m_screenManager;					/* Screen manager */
		IGameScreen* m_currentScreen = nullptr;			/* current game screen */
		GameWindow m_window;							/* Game window */
		EventHandler m_eventHandler;					/* input manager */
		Gui m_gui;										/* Gui manager */
		bool m_isRunning = false;						/* Keep the state of game */

	private:
		FPSLimiter m_limiter;
	};
}
