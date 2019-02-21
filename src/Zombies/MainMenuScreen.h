#pragma once

#include "ScreenIndices.h"
#include "GuiMainMenu.h"
#include "App.h"

#include <IGameScreen.h>
#include <Camera2D.h>

class MainMenuScreen : public NS2::IGameScreen
{
public:
	friend class GuiMainMenu;
	friend class GuiMenuConfig;

	MainMenuScreen();

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

	virtual int getNextSCreenIndex() const override;
	virtual int getPreviousSCreenIndex() const override;

	void setNextSCreenIndex(int index){ m_nextScreenIndex = index; }

private:
	void initGui();
	void checkInput();

	NS2::Camera2D m_camera;
	GuiMainMenu m_menu;
	int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
};

