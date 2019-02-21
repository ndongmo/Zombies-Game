/*!
* \file EventHandler.h
* \brief Event handler class, it checks and stores events.
* \author Ndongmo Silatsa Fabrice
* \date 24-10-2016
* \version 1.0
*/

#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL/SDL.h>


namespace NS2
{

#define JOY_DEAD_ZONE 2000
#define JOY_AXE_MAX  32767
#define MARGIN 500

	struct EventConfig{
		EventConfig() :keyID(0), joyID(0){}
		EventConfig(unsigned int key, unsigned int joy) :keyID(key), joyID(joy){}
		unsigned int keyID;
		unsigned int joyID;
	};

	/*!
	* \brief Allow to pick what user wants to play with.
	*/
	enum class PlayWith{ KEYBOARD, GAMEPAD };

	/*!
	* \brief Joystick events (named according XBOX 360 Game pad).
	*/
	enum Joystick
	{
		A = 10 + MARGIN, B = 11 + MARGIN, X = 12 + MARGIN, Y = 13 + MARGIN, HOME = 14 + MARGIN, LB = 8 + MARGIN, RB = 9 + MARGIN, 
		L3 = 6 + MARGIN, R3 = 7 + MARGIN, SELECT = 5 + MARGIN, START = 4 + MARGIN, DPAD_UP = 0 + MARGIN, DPAD_DOWN = 1 + MARGIN, 
		DPAD_LEFT = 2 + MARGIN, DPAD_RIGHT = 3 + MARGIN, LT = 15 + MARGIN, RT = 16 + MARGIN, AXE1_UP = 17 + MARGIN, 
		AXE1_DOWN = 18 + MARGIN, AXE1_LEFT = 19 + MARGIN, AXE1_RIGHT = 20 + MARGIN, AXE2_UP = 21 + MARGIN, AXE2_DOWN = 22 + MARGIN, 
		AXE2_LEFT = 23 + MARGIN, AXE2_RIGHT = 24 + MARGIN, NOTHING = 25 + MARGIN
	};

	class GameWindow;

	class EventHandler
	{
	public:
		/*!
		* \brief Initialize joystick SDL stuffs.
		*/
		void init(int screenWidth, int screenHeight);
		/*!
		* \brief Free SDL joystick resources.
		*/
		void destroy();
		/*!
		* \brief Check and update list of events.
		*/
		void update(float deltaTime);
		/*!
		* \brief Update joystick axes.
		* \param axe: concerned axe
		* \param value : incline
		* \param window : current SDL window
		*/
		void updateJoystickAxis(int axe, int value, SDL_Window* window);
		/*!
		* \brief Update joystick axes.
		* \param value: wich value (UP, LEFT, ...)
		*/
		void updateJoystickHats(int value);
		/*!
		* \brief Handle key pressed.
		* \param keyID: ID of pressed key
		*/
		void pressKey(unsigned int keyID);
		/*!
		* \brief Handle key released.
		* \param keyID: ID of released key
		*/
		void releaseKey(unsigned int keyID);
		/*!
		* \brief Handle key down action.
		* \param keyID: key ID
		*/
		bool isKeyDown(unsigned int keyID);
		/*
		* \brief Check if key has just pressed.
		* \param keyID: key ID
		* \return true if key has just pressed
		*/
		bool isKeyPressed(unsigned int keyID);
		/*!
		* \brief Handle event down action.
		* \param eventID: ID of event
		*/
		bool isEventDown(unsigned int eventID);
		/*
		* \brief Check if event has just occured.
		* \param eventID: ID of event
		* \return true if event has just occured
		*/
		bool isEventPressed(unsigned int eventID);
		/*!
		* \brief Handle key released.
		* \param keyID: ID of released key
		*/
		void releaseEvent(unsigned int eventID);
		/*
		* \brief Check if joystick is connected.
		*/
		bool isJoyConnect(){ return m_joystick != nullptr; }
		/*
		* \brief Add or set event.
		* \param eventID: ID of event
		*/
		void addEvent(unsigned int eventID, unsigned int keyID, unsigned int joyID)
		{
			m_eventConfig[eventID] = EventConfig(keyID, joyID);
		}
		/*
		* \brief Update mapping with pressed key.
		* \param eventID: ID of event
		*/
		void updateMapping(unsigned int eventID, PlayWith util);
		/*
		* \brief Get the key's name.
		* \param eventID: ID of event
		* \return name of key, joystick or mouse button
		*/
		std::string getMapping(unsigned int eventID, PlayWith util);
		/*
		* \brief Get the key's name.
		* \param eventID: ID of event
		* \return name of key, joystick or mouse button
		*/
		std::string getMapping(unsigned int eventID);

		void updateConfig();
		void saveConfig();
		void clearConfig();
		bool loadConfigFile();
		bool saveConfigFile();

		/* SETTERS */
		void setMouseCoords(float x, float y);
		void setPlayWith(PlayWith util){ m_playWith = util; }
		/* GETTERS */
		glm::vec2 getMouseCoords() const { return m_mouseCoords; }
		PlayWith getPlayWith(){ return m_playWith; }
		EventConfig getEventKey(unsigned int eventID)
		{ 
			return m_eventConfig[eventID]; 
		}

	private:
		/*!
		* \brief Check if key was down.
		* \param keyID: ID of key
		*/
		bool wasKeyDown(unsigned int keyID);
		std::string getMapping(unsigned int eventID, PlayWith util, std::unordered_map<unsigned int, EventConfig>& map);

		std::unordered_map<unsigned int, EventConfig> m_eventConfig;		/* configuration of keyboard and gamepad */
		std::unordered_map<unsigned int, EventConfig> m_eventConfigTemp;	/* Allow user to configure keyboard or gamepad */
		std::unordered_map<unsigned int, bool> m_keyMap;					/* Map of current key ID */
		std::unordered_map<unsigned int, bool> m_previousKeyMap;			/* Map of previous key ID */
		glm::vec2 m_mouseCoords = glm::vec2(0);								/* Mouse coordinates */
		glm::vec2 m_joyAxeValue = glm::vec2(0);								/* Axe values */
		PlayWith m_playWith = PlayWith::KEYBOARD;							/* util to play with */
		SDL_Joystick * m_joystick = nullptr;								/* Pointer to joystick structure */
		int m_screenWidth, m_screenHeight;
		float m_axisTimeLapse = 0.0f;
	};
}