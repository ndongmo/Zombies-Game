/*!
 * \file ScreenManager.h
 * \brief Screens manager.
 *        It contains all game screens and manges moving on different screens.
 * \author Ndongmo Silatsa Fabrice
 * \date 24-10-2016
 * \version 1.0
 */

#pragma once

#include <vector>

namespace NS2
{
#define SCREEN_INDEX_NO_SCREEN -1

	class IGameScreen;
	class IMainGame;

	class ScreenManager
	{
	public:

		/*!
		* \brief Move to next screen.
		* \return a pointer to next game screen.
		*/
		IGameScreen* moveNext();
		/*!
		* \brief Move to previous screen.
		* \return a pointer to previous game screen.
		*/
		IGameScreen* movePrevious();
		/*!
		* \brief Add new game screen.
		* \param screen: a pointer to game screen to add.
		*/
		void addScreen(IGameScreen* screen);
		/*!
		* \brief Change current screen.
		* \param screenIndex: index of new current game screen.
		*/
		void setScreen(int screenIndex);
		/*!
		* \brief Initialize screen manager.
		* \param game: a pointer to game main object.
		*/
		void init(IMainGame* game);
		/*!
		* \brief Destroy all game screens.
		*/
		void destroy();

		/* GETTERS */
		IGameScreen* getCurrentScreen();

	protected:
		IMainGame* m_game = nullptr;						/* Pointer to game main object */
		std::vector<IGameScreen*> m_screens;				/* Vector of game screens */
		int m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;	/* Current screen index */
	};
}