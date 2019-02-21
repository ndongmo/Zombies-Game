#pragma once

#include <IGuiWindow.h>

class MainMenuScreen;

class GuiMainMenu : public NS2::IGuiWindow
{
public:
	GuiMainMenu(MainMenuScreen* parent) : m_parent(parent){ }

	virtual void initWithGui(NS2::Gui& gui) override;

	/* Event handler methods */
	bool onExitCliked(const CEGUI::EventArgs& e);
	bool onGameCliked(const CEGUI::EventArgs& e);
	bool onEditorCliked(const CEGUI::EventArgs& e);
	bool onConfigCliked(const CEGUI::EventArgs& e);

private:
	MainMenuScreen* m_parent = nullptr;
};

