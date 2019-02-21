#include "GuiMainMenu.h"
#include "GuiMenuConfig.h"
#include "MainMenuScreen.h"

#include <ResourceManager.h>

void GuiMainMenu::initWithGui(NS2::Gui& gui)
{
	m_window = gui.createWindow("NS_mainMenu.layout");

	CEGUI::PushButton* bt_game = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_game"));
	bt_game->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiMainMenu::onGameCliked, this));

	CEGUI::PushButton* bt_editor = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_edit"));
	bt_editor->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiMainMenu::onEditorCliked, this));

	CEGUI::PushButton* bt_config = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_config"));
	bt_config->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiMainMenu::onConfigCliked, this));

	CEGUI::PushButton* bt_quit = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_quit"));
	bt_quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiMainMenu::onExitCliked, this));
}

bool GuiMainMenu::onExitCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/click.mp3").play();
	m_parent->setGameState(NS2::ScreenState::EXIT_GAME);
	return true;
}

bool GuiMainMenu::onGameCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	m_parent->setNextSCreenIndex(SCREEN_INDEX_GAMEPLAY);
	m_parent->setGameState(NS2::ScreenState::CHANGE_NEXT);
	return true;
}

bool GuiMainMenu::onEditorCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	m_parent->setNextSCreenIndex(SCREEN_INDEX_EDITOR);
	m_parent->setGameState(NS2::ScreenState::CHANGE_NEXT);
	return true;
}

bool GuiMainMenu::onConfigCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	CEGUI::Window* w = m_parent->m_game->getGui().createWindow("NS_config.layout");
	GuiMenuConfig* gui = new GuiMenuConfig(m_parent);
	gui->initWithWindow(w);
	gui->setPreviousWindow(this);
	setNextWindow(gui);
	next();
	return true;
}