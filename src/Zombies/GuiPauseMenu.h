#pragma once

#include "Bag.h"

#include <IGuiWindow.h>

class GameScreen;

class GuiPauseMenu : public NS2::IGuiWindow
{
public:
	GuiPauseMenu(GameScreen* parent) : m_parent(parent){ }

	virtual void initWithGui(NS2::Gui& gui) override;

	void update();

	/* Event handler methods */
	bool onExitCliked(const CEGUI::EventArgs& e);
	bool onCloseCliked(const CEGUI::EventArgs& e);
	bool onEquipCliked(const CEGUI::EventArgs& e);
	bool onUseCliked(const CEGUI::EventArgs& e);
	bool onDropCliked(const CEGUI::EventArgs& e);
	bool onTileCliked(const CEGUI::EventArgs& e);

private:
	std::string getImageRsc(Object* obj);

	GameScreen* m_parent = nullptr;

	CEGUI::ScrollablePane* m_layout = nullptr;
	CEGUI::Window* tx_descr = nullptr;
	CEGUI::PushButton* bt_exit = nullptr;
	CEGUI::PushButton* bt_close = nullptr;
	CEGUI::PushButton* bt_equip = nullptr;
	CEGUI::PushButton* bt_use = nullptr;
	CEGUI::PushButton* bt_drop = nullptr;
	CEGUI::Window* m_currentTile = nullptr;
};

