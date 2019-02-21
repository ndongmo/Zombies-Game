#pragma once

#include "MainMenuScreen.h"


#include <thread>
#include <IGuiWindow.h>

class GuiMenuConfig : public NS2::IGuiWindow
{
public:
	GuiMenuConfig(MainMenuScreen* parent) : m_parent(parent){ }

	virtual void initWithWindow(CEGUI::Window* window) override;
	virtual void destroy()override;

	void updateConfig();

	void run();

	/* Event handler methods */
	bool onSaveCliked(const CEGUI::EventArgs& e);
	bool onCancelCliked(const CEGUI::EventArgs& e);
	bool onKeyboarChecked(const CEGUI::EventArgs& e);
private:
	std::thread* m_keyLoop;
	bool m_runningLoop = true;
	MainMenuScreen* m_parent = nullptr;

	CEGUI::RadioButton* rb_reload;
	CEGUI::RadioButton* rb_run;
	CEGUI::RadioButton* rb_pause;
	CEGUI::RadioButton* rb_attack;
	CEGUI::RadioButton* rb_action;
	CEGUI::RadioButton* rb_changeW;
	CEGUI::RadioButton* rb_changeO;
	CEGUI::RadioButton* rb_useO;
	CEGUI::RadioButton* rb_up;
	CEGUI::RadioButton* rb_down;
	CEGUI::RadioButton* rb_left;
	CEGUI::RadioButton* rb_right;
	CEGUI::RadioButton* rb_joystick;
	CEGUI::RadioButton* rb_keyboard;
};

