#include "GameScreen.h"

#include "ScreenIndices.h"
#include "XmlExtractor.h"

#include "Zombie.h"
#include "Gun.h"
#include "Light.h"
#include "Human.h"
#include "EventConfig.h"

#include <algorithm>
#include <iostream>
#include <ctime>
#include <random>
#include <SDL/SDL.h>
#include <glm\gtx\rotate_vector.hpp>
#include <ResourceManager.h>
#include <FPSLimiter.h>
#include <Logger.h>

GameScreen::GameScreen() : m_currentLevel(m_particleEngine), m_pauseMenu(this)
{
	m_inPause = false;
	m_screenIndex = SCREEN_INDEX_GAMEPLAY;
	m_gameState = GameState::PLAY;
	m_previousTiks = (float)SDL_GetTicks();
	m_renderer.setDebug(false);
}

GameScreen::~GameScreen()
{
}

void GameScreen::build()
{

}
void GameScreen::destroy()
{

}

bool GameScreen::onExitCliked(const CEGUI::EventArgs& e)
{
	setGameState(NS2::ScreenState::CHANGE_PREVIOUS);
	return true;
}

void GameScreen::onEntry()
{
	initSystems();

	m_pauseMenu.initWithGui(m_game->getGui());
	m_pauseMenu.getWindow()->setVisible(false);

	initLevel();
	m_game->getGui().hideMouseCursor();
	m_inPause = false;
}

void GameScreen::onExit()
{
	m_game->getGui().clear();
	m_renderer.dispose();
	m_game->getGui().showMouseCursor();
}

void GameScreen::initSystems() {
	m_screenWidth = m_game->getWindow().getScreenWidth();
	m_screenHeight = m_game->getWindow().getScreenHeight();

	m_camera.init(m_screenWidth, m_screenHeight);
	m_camera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	m_renderer.init(m_screenWidth, m_screenHeight);
}

void GameScreen::initLevel() {
	std::string error;
	XmlExtractor::loadLevel(m_currentLevel, "Levels/level_1.xml", error,
		&m_game->getEventHandler(), &m_camera);
	m_currentLevel.update();
	m_renderer.clearAmbientColor(m_currentLevel.ambientColor);
}


void GameScreen::update()
{
	checkInput();

	if (!m_inPause)
	{
		int i = 0;
		float newTicks, frameTime, totalDeltaTime;

		newTicks = (float)SDL_GetTicks();
		frameTime = newTicks - m_previousTiks;
		m_previousTiks = newTicks;
		totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = (totalDeltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : totalDeltaTime;

			m_entityController.updateEntities(m_currentLevel, deltaTime);
			m_particleEngine.update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}

		m_camera.setPosition(m_currentLevel.getPlayer().getShape().position);
		m_camera.update();

		if (!m_currentLevel.player.isAlive())
			onExitCliked(CEGUI::EventArgs());
	}
}

void GameScreen::draw()
{
	m_renderer.render(m_currentLevel, m_camera, m_particleEngine);
}

int GameScreen::getNextSCreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

int GameScreen::getPreviousSCreenIndex() const
{
	return SCREEN_INDEX_MAINMENU;
}

void GameScreen::checkInput()
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
	{
		if (m_currentLevel.input->isEventDown(MyEvent::PAUSE)) {
			m_currentLevel.input->releaseEvent(MyEvent::PAUSE);
			if (m_inPause) {
				m_pauseMenu.onCloseCliked(CEGUI::EventArgs());
				m_inPause = false;
				m_game->getGui().hideMouseCursor();
			}
			else {
				NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
				m_inPause = true;
				m_pauseMenu.update();
				m_pauseMenu.getWindow()->setVisible(true);
				m_game->getGui().showMouseCursor();
			}
		}
		m_game->onSDLEvent(ev);
	}
}
void GameScreen::setPause(bool pause)
{
	m_inPause = pause;
	if (m_inPause)
		m_game->getGui().showMouseCursor();
	else
		m_game->getGui().hideMouseCursor();
}