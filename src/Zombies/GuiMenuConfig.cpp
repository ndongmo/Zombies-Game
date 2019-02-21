#include "GuiMenuConfig.h"
#include "EventConfig.h"
#include "MainMenuScreen.h"

#include <string>
#include <ResourceManager.h>


void GuiMenuConfig::initWithWindow(CEGUI::Window* window)
{
	m_window = window;

	CEGUI::PushButton* bt_save = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_save"));
	bt_save->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiMenuConfig::onSaveCliked, this));

	CEGUI::PushButton* bt_cancel = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_cancel"));
	bt_cancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiMenuConfig::onCancelCliked, this));

	rb_joystick = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_playWith/rb_joystick"));
	rb_keyboard = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_playWith/rb_keyboard"));
	rb_keyboard->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiMenuConfig::onKeyboarChecked, this));

	rb_reload = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_reload"));
	rb_run = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_run"));
	rb_pause = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_pause"));
	rb_attack = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_attack"));
	rb_action = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_action"));
	rb_changeW = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_changeW"));
	rb_changeO = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_changeO"));
	rb_useO = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_useO"));
	rb_up = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_up"));
	rb_down = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_down"));
	rb_left = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_left"));
	rb_right = static_cast<CEGUI::RadioButton*> (m_window->getChild("gb_keyConfig/rb_right"));

	m_parent->m_game->getEventHandler().updateConfig();

	if (!m_parent->m_game->getEventHandler().isJoyConnect())
		rb_joystick->disable();

	bool flag = m_parent->m_game->getEventHandler().getPlayWith() == NS2::PlayWith::GAMEPAD;
	rb_joystick->setSelected(flag);
	rb_keyboard->setSelected(!flag);

	updateConfig();

	m_keyLoop = new std::thread(&GuiMenuConfig::run, this);
}

void GuiMenuConfig::destroy()
{
	try {
		m_runningLoop = false;
		NS2::IGuiWindow::destroy();
		m_keyLoop->join();
		
	} catch (std::exception e) {}
}

void GuiMenuConfig::updateConfig()
{
	std::string text = "";
	NS2::PlayWith util = (rb_keyboard->isSelected()) ? NS2::PlayWith::KEYBOARD : NS2::PlayWith::GAMEPAD;

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::RELOAD, util);
	CEGUI::Window* lb_reload = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_reload"));
	if (text != "") lb_reload->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::RUN, util);
	CEGUI::Window* lb_run = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_run"));
	if (text != "") lb_run->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::PAUSE, util);
	CEGUI::Window* lb_pause = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_pause"));
	if (text != "") lb_pause->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::ATTACK, util);
	CEGUI::Window* lb_shoot = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_attack"));
	if(text != "") lb_shoot->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::ACTION, util);
	CEGUI::Window* lb_weapon = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_action"));
	if (text != "") lb_weapon->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::CHANGE_WEAPON, util);
	CEGUI::Window* lb_changeW = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_changeW"));
	if (text != "") lb_changeW->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::CHANGE_OBJECT, util);
	CEGUI::Window* lb_changeO = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_changeO"));
	if (text != "") lb_changeO->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::USE_OBJECT, util);
	CEGUI::Window* lb_useO = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_useO"));
	if (text != "") lb_useO->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::GO_UP, util);
	CEGUI::Window* lb_up = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_up"));
	if (text != "") lb_up->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::GO_DOWN, util);
	CEGUI::Window* lb_down = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_down"));
	if (text != "") lb_down->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::GO_LEFT, util);
	CEGUI::Window* lb_left = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_left"));
	if (text != "") lb_left->setText(text);

	text = m_parent->m_game->getEventHandler().getMapping(MyEvent::GO_RIGHT, util);
	CEGUI::Window* lb_right = static_cast<CEGUI::Window*> (m_window->getChild("gb_keyConfig/lb_right"));
	if (text != "") lb_right->setText(text);
}

void GuiMenuConfig::run()
{
	m_runningLoop = true;
	try
	{
		while (m_runningLoop){
			NS2::PlayWith util = (rb_keyboard->isSelected()) ? NS2::PlayWith::KEYBOARD : NS2::PlayWith::GAMEPAD;

			if (rb_attack->isSelected()){
				if (rb_keyboard->isSelected())
					rb_attack->disable();
				else
				{
					rb_attack->enable();
					m_parent->m_game->getEventHandler().updateMapping(MyEvent::ATTACK, util);
				}
			}
			else if (rb_pause->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::PAUSE, util);
			}
			else if (rb_reload->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::RELOAD, util);
			}
			else if (rb_run->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::RUN, util);
			}
			else if (rb_action->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::ACTION, util);
			}
			else if (rb_changeW->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::CHANGE_WEAPON, util);
			}
			else if (rb_changeO->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::CHANGE_OBJECT, util);
			}
			else if (rb_useO->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::USE_OBJECT, util);
			}
			else if (rb_up->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::GO_UP, util);
			}
			else if (rb_down->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::GO_DOWN, util);
			}
			else if (rb_left->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::GO_LEFT, util);
			}
			else if (rb_right->isSelected()){
				m_parent->m_game->getEventHandler().updateMapping(MyEvent::GO_RIGHT, util);
			}

			updateConfig();
		}
	}
	catch (std::exception ex) { }
}

bool GuiMenuConfig::onSaveCliked(const CEGUI::EventArgs& e)
{
	if (m_parent->m_game->getEventHandler().isJoyConnect() && rb_joystick->isSelected())
		m_parent->m_game->getEventHandler().setPlayWith(NS2::PlayWith::GAMEPAD);
	else 
		m_parent->m_game->getEventHandler().setPlayWith(NS2::PlayWith::KEYBOARD);

	m_parent->m_game->getEventHandler().saveConfig();
	
	if (m_keyLoop != nullptr && m_keyLoop->joinable())
	{
		m_runningLoop = false;
		m_keyLoop->join();
	}
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	m_parent->m_game->getEventHandler().clearConfig();
	previous();
	return true;
}

bool GuiMenuConfig::onCancelCliked(const CEGUI::EventArgs& e)
{
	if (m_keyLoop != nullptr && m_keyLoop->joinable())
	{
		m_runningLoop = false;
		m_keyLoop->join();
	}
	NS2::ResourceManager::getSound("Sounds/click.mp3").play();
	m_parent->m_game->getEventHandler().clearConfig();
	previous();
	return true;
}

bool GuiMenuConfig::onKeyboarChecked(const CEGUI::EventArgs& e)
{
	if (rb_joystick->isSelected())
		m_parent->m_game->getEventHandler().setPlayWith(NS2::PlayWith::GAMEPAD);
	else
		m_parent->m_game->getEventHandler().setPlayWith(NS2::PlayWith::KEYBOARD);
	updateConfig();
	return true;
}