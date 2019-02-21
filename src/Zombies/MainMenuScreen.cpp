#include "MainMenuScreen.h"

#include <ScreenManager.h>
#include <IGameScreen.h>
#include <ResourceManager.h>


MainMenuScreen::MainMenuScreen() :m_menu(this)
{
	m_screenIndex = SCREEN_INDEX_MAINMENU;
}


void MainMenuScreen::build()
{
}

void MainMenuScreen::destroy()
{
	m_menu.destroy();
}

void MainMenuScreen::onEntry()
{
	m_camera.init(m_game->getWindow().getScreenWidth(), m_game->getWindow().getScreenHeight());
	m_camera.setScale(10.0f);
	initGui();
	NS2::ResourceManager::getMusic("Musics/XYZ.ogg").play();
}

void MainMenuScreen::onExit()
{
	m_game->getGui().clear();
	NS2::ResourceManager::getMusic("Musics/XYZ.ogg").stop();
}

void MainMenuScreen::update()
{
	m_camera.update();
	checkInput();
}

void MainMenuScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int MainMenuScreen::getNextSCreenIndex() const
{
	return m_nextScreenIndex;
}

int MainMenuScreen::getPreviousSCreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

void MainMenuScreen::initGui()
{
	m_menu.initWithGui(m_game->getGui());
}

void MainMenuScreen::checkInput()
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
	{
		m_game->onSDLEvent(ev);
	}

}