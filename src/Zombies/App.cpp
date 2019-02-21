#include "App.h"
#include "GameScreen.h"
#include "MainMenuScreen.h"
#include "EditorScreen.h"
#include "EventConfig.h"
#include "XmlExtractor.h"

#include <ScreenManager.h>

App::App() : IMainGame()
{
}

App::~App()
{

}

void App::onInit()
{
	/* Add events for keyboard and joystick */
	if (!m_eventHandler.loadConfigFile())
	{
		m_eventHandler.addEvent(MyEvent::PAUSE, SDLK_p, NS2::Joystick::START);
		m_eventHandler.addEvent(MyEvent::RUN, SDLK_SPACE, NS2::Joystick::LB);
		m_eventHandler.addEvent(MyEvent::RELOAD, SDLK_r, NS2::Joystick::RB);
		m_eventHandler.addEvent(MyEvent::ATTACK, SDL_BUTTON_LEFT, NS2::Joystick::DPAD_LEFT);
		m_eventHandler.addEvent(MyEvent::ACTION, SDL_BUTTON_RIGHT, NS2::Joystick::DPAD_RIGHT);
		m_eventHandler.addEvent(MyEvent::CHANGE_OBJECT, SDLK_j, NS2::Joystick::DPAD_LEFT);
		m_eventHandler.addEvent(MyEvent::CHANGE_WEAPON, SDLK_k, NS2::Joystick::DPAD_RIGHT);
		m_eventHandler.addEvent(MyEvent::USE_OBJECT, SDLK_l, NS2::Joystick::DPAD_UP);
		m_eventHandler.addEvent(MyEvent::GO_UP, SDLK_w, NS2::Joystick::AXE1_UP);
		m_eventHandler.addEvent(MyEvent::GO_DOWN, SDLK_s, NS2::Joystick::AXE1_DOWN);
		m_eventHandler.addEvent(MyEvent::GO_LEFT, SDLK_a, NS2::Joystick::AXE1_LEFT);
		m_eventHandler.addEvent(MyEvent::GO_RIGHT, SDLK_d, NS2::Joystick::AXE1_RIGHT);
	}

	/* Initialize gui stuff */
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.setFont("Jura-10");
	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);
}

void App::onExit()
{

}

void App::addScreens()
{
	m_gameplayScreen = std::make_unique<GameScreen>();
	m_mainMenuScreen = std::make_unique<MainMenuScreen>();
	m_editorScreen = std::make_unique<EditorScreen>();

	m_screenManager.addScreen(m_mainMenuScreen.get());
	m_screenManager.addScreen(m_gameplayScreen.get());
	m_screenManager.addScreen(m_editorScreen.get());

	m_screenManager.setScreen(m_mainMenuScreen->getScreenIndex());
}
