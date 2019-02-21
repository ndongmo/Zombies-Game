/*!
* \file Gui.h
* \brief GUI manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <string>
#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>

namespace NS2
{
	class Gui
	{
	public:
		/*!
		* \brief Initialize CEGUI context.
		* \param resourceDirectory :  CEGUI resource directory.
		* \return true for no problem, otherwise return false.
		*/
		bool init(const std::string& resourceDirectory);
		/*!
		* \brief clear root children.
		*/
		void clear();
		/*!
		* \brief Free all CEGUI resources.
		*/
		void destroy();
		/*!
		* \brief Draw all current GUI.
		*/
		void draw();
		/*!
		* \brief Update all current GUI.
		*/
		void update();
		/*!
		* \brief Inject SDL events to CEGUI context.
		* \param ev : SDL event
		*/
		void onSDLEvent(SDL_Event& ev);

		/*!
		* \brief Return the corresponding CEGUI key input from a SDL key input.
		* \param key SDL key.
		* \return CEGUI key code.
		*/
		CEGUI::Key::Scan sdlkeyTOceguikey(SDL_Keycode key);
		/*!
		* \brief Return the corresponding CEGUI mouse button from a SDL mouse button.
		* \param sdlButton SDL mouse button.
		* \return CEGUI mouse button code.
		*/
		CEGUI::MouseButton sdlbuttonTOceguibutton(Uint8 sdlButton);
		/*!
		* \brief Return the corresponding CEGUI touch from a SDL joystick button.
		* \param ev : SDL event
		*/
		void sdlJoystickTOcegui(SDL_Event& ev);
		/*!
		* \brief Méthode qui charge un fichier scheme.
		* \param scheme: fichier scheme.
		*/
		void loadScheme(const std::string& schemeFile);
		/*!
		* \brief Méthode qui charge un fichier image contenat un ensemble d'images.
		* \param imagesetName: fichier à charger.
		*/
		void loadImageset(const std::string& imagesetName);
		/*!
		* \brief Enable drawing mouse cursor.
		*/
		void showMouseCursor();
		/*!
		* \brief Disable drawing mouse cursor.
		*/
		void hideMouseCursor();

		/* SETTERS */
		void setMouseCursor(const std::string& cursorFile);
		void setFont(const std::string& fontFile);
		void setMousePosition(const glm::vec2& mousPosition){
			m_context->injectMousePosition(mousPosition.x, mousPosition.y);
		}

		/*!
		* \brief Méthode qui crée une image à partir d'un imageset.
		* \param type: type de l'image.
		* \param name: nom de l'image.
		*/
		CEGUI::Image& creatImage(const std::string& type, const std::string& name);

		/*!
		* \brief Méthode qui crée une fenêtre.
		* \param scheme: fichier du thème.
		* \param name: nom de la fenêtre.
		* \param parent: parent de la fenêtre.
		*/
		CEGUI::Window* createWindow(const std::string& scheme, const glm::vec4& destRectPerc, const glm::vec4& destRectPix,
			const std::string& name = "");
		CEGUI::Window* createWindow(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc,
			const glm::vec4& destRectPix, const std::string& name = "");
		CEGUI::Window* createWindow(CEGUI::Window* parent, const std::string& layout);
		CEGUI::Window* createWindow(const std::string& layout);


		static void setWindowDestRect(CEGUI::Window* window, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);
		static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
		const CEGUI::GUIContext* getContext() { return m_context; }

	private:
		static CEGUI::OpenGL3Renderer* m_renderer;	/* OpenGL3 Renderer */
		CEGUI::GUIContext* m_context = nullptr;		/* CEGUI context */
		CEGUI::Window* m_root = nullptr;			/* CEGUI root window */
		unsigned int m_lastTime = 0;				/* Last frame time */
	};
}

