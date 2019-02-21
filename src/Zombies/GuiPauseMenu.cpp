#include "GuiPauseMenu.h"
#include "GameScreen.h"
#include "Ammo.h"

#include <ResourceManager.h>

void GuiPauseMenu::initWithGui(NS2::Gui& gui)
{
	gui.loadImageset("NS_Look.imageset");
	m_window = gui.createWindow("NS_pauseMenu.layout");
	m_layout = static_cast<CEGUI::ScrollablePane*>(m_window->getChild("layout"));
	tx_descr = m_window->getChild("tx_desc");
	bt_exit = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_exit"));
	bt_exit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiPauseMenu::onExitCliked, this));
	bt_close = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_close"));
	bt_close->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiPauseMenu::onCloseCliked, this));
	bt_equip = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_equip"));
	bt_equip->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiPauseMenu::onEquipCliked, this));
	bt_use = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_use"));
	bt_use->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiPauseMenu::onUseCliked, this));
	bt_drop = static_cast<CEGUI::PushButton*> (m_window->getChild("bt_drop"));
	bt_drop->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiPauseMenu::onDropCliked, this));
}

std::string GuiPauseMenu::getImageRsc(Object* obj)
{
	if (obj != nullptr)
	{
		if (obj->getObjectType() == ObjectType::AMMO) {
			GunType type = (static_cast<Ammo*>(obj))->getGunType();
			if (type == GunType::HANDGUN) return "ammo_1";
			else if (type == GunType::RIFLE) return "ammo_2";
			else return "ammo_3";
		}
		else if (obj->getObjectType() == ObjectType::GUN) {
			GunType type = (static_cast<Gun*>(obj))->getGunType();
			if (type == GunType::HANDGUN) return "pistol";
			else if (type == GunType::RIFLE) return "rifle";
			else return "shotgun";
		}
		else if (obj->getObjectType() == ObjectType::MEDICINE) {
			return "medicine";
		}
		else if (obj->getObjectType() == ObjectType::BAG) {
			return "bag";
		}
		else if (obj->getObjectType() == ObjectType::FLASHLIGHT) {
			return "flashlight";
		}
		else if (obj->getObjectType() == ObjectType::KEY) {
			return "key";
		}
	}

	return "background";
}
void GuiPauseMenu::update()
{
	Bag& bag = m_parent->getPlayer().getBag();
	const size_t cols = 6;
	const float width = 100;
	const float height = 100;
	const float margin = 5;
	int bagSize = bag.getColumns() * bag.getRows();
	size_t rows = (bagSize / cols) + ((bagSize % cols > 0) ? 1 : 0);

	while (m_layout->getChildAtIdx(0)->getChildCount() > 0) {
		CEGUI::Window* child = m_layout->getChildAtIdx(0)->getChildAtIdx(0);
		m_layout->getChildAtIdx(0)->removeChild(child);
	}

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++)
		{
			Object* obj = bag.getObject(j, i);
			CEGUI::UDim x(0, j * (width + margin));
			CEGUI::UDim y(0, i * (height + margin));
			CEGUI::UDim w(0, width);
			CEGUI::UDim h(0, height);

			CEGUI::Window* back = m_window->createChild("TaharezLook/StaticImage");
			back->setArea(x, y, w, h);
			back->setProperty("Image", "NS_Look/background");
			back->setID((i*cols + j) * 2);

			m_layout->addChild(back);

			if (obj != nullptr) {
				CEGUI::Window* tile = m_window->createChild("TaharezLook/StaticImage");
				tile->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&GuiPauseMenu::onTileCliked, this));
				tile->setArea(x, y, w, h);
				tile->setID(((i*cols + j) *2) + 1);
				tile->setProperty("BackgroundEnabled", "false");
				tile->setTooltipText(obj->getName());
				tile->setProperty("Image", "NS_Look/" + getImageRsc(obj));
				tile->setUserData(obj);
				back->setUserData(obj);
				m_layout->addChild(tile);
			}
		}
	}
}

bool GuiPauseMenu::onExitCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	m_window->setVisible(false);
	m_parent->onExitCliked(CEGUI::EventArgs());
	return true;
}
bool GuiPauseMenu::onCloseCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	m_window->setVisible(false);
	m_parent->setPause(false);
	return true;
}
bool GuiPauseMenu::onEquipCliked(const CEGUI::EventArgs& e)
{
	Object* obj = static_cast<Object*>(m_currentTile->getUserData());
	m_parent->getPlayer().equip(obj);
	return true;
}

bool GuiPauseMenu::onUseCliked(const CEGUI::EventArgs& e)
{
	Object* obj = static_cast<Object*>(m_currentTile->getUserData());
	m_parent->getPlayer().equip(obj);
	if (m_parent->getPlayer().useObject(m_parent->getLevel())) {
		m_layout->getChild(0)->removeChild(m_currentTile->getID() + 1);
		m_currentTile->setVisible(true);
		m_currentTile->setUserData(nullptr);
		m_currentTile->removeAllEvents();
	}
	return true;
}

bool GuiPauseMenu::onDropCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/drop.ogg").play();
	Object* obj = static_cast<Object*>(m_currentTile->getUserData());
	m_parent->getPlayer().removeObject(obj);
	m_layout->getChild(0)->removeChild(m_currentTile->getID() + 1);
	m_currentTile->setVisible(true);
	m_currentTile->setUserData(nullptr);
	m_currentTile->removeAllEvents();
	return true;
}

bool GuiPauseMenu::onTileCliked(const CEGUI::EventArgs& e)
{
	NS2::ResourceManager::getSound("Sounds/ui_click.ogg").play();
	const CEGUI::MouseEventArgs& me = static_cast<const CEGUI::MouseEventArgs&>(e);
	if (me.window->getUserData() != nullptr)
	{
		size_t id = me.window->getID();

		if (m_currentTile != nullptr) {
			m_currentTile->setVisible(true);
		}
		m_currentTile = m_layout->getChild(0)->getChild(id - 1);
		m_currentTile->setVisible(false);

		Object* obj = static_cast<Object*>(m_currentTile->getUserData());
		std::string str = "";
		obj->textValue(str);
		if (!str.empty()) str = " [" + str + "]";
		tx_descr->setText("Name : " + obj->getName() + str);
		bt_drop->setEnabled(true);
		if (obj->getObjectType() == ObjectType::GUN) {
			bt_use->setEnabled(false);
			bt_equip->setEnabled(true);
		}
		else if (obj->getObjectType() == ObjectType::AMMO) {
			bt_use->setEnabled(false);
			bt_equip->setEnabled(false);
		}
		else {
			bt_use->setEnabled(true);
			bt_equip->setEnabled(true);
			if (obj->getObjectType() == ObjectType::KEY)
				bt_drop->setEnabled(false);
		}
	}
	
	return true;
}

