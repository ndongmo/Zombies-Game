/*!
* \file IMainGame.h
* \brief This class represent the main game class.
*        It contains all game stuffs and runs game main process.
* \author Ndongmo Silatsa Fabrice
* \date 24-10-2016
* \version 1.0
*/

#pragma once

#include "IMainGame.h"
#include "IGameScreen.h"

#include <CEGUI\CEGUI.h>

namespace NS2
{
	class IGuiWindow
	{
	public:
		virtual ~IGuiWindow();
		/*!
		* \brief Initialize window.
		*/
		virtual void initWithGui(Gui& gui) {}
		virtual void initWithWindow(CEGUI::Window* window) {}
		/*!
		* \brief Destroy window.
		*/
		virtual void destroy();
		/*!
		* \brief Destroy next window.
		*/
		virtual void destroyNext();
		/*!
		* \brief Back to previous window.
		*/
		virtual void previous();
		/*!
		* \brief Go to next window.
		*/
		virtual void next();

		/* SETTERS */
		void setNextWindow(IGuiWindow* nextWindow){ m_nextWindow = nextWindow; }
		void setPreviousWindow(IGuiWindow* previousWindow){ m_previousWindow = previousWindow; }
		/* GETTERS */
		CEGUI::Window* getWindow(){ return m_window; }

	protected:
		CEGUI::Window* m_window = nullptr;
		IGuiWindow* m_nextWindow = nullptr;
		IGuiWindow* m_previousWindow = nullptr;
	};
}

