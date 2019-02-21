#include "GuiEditor.h"
#include "EditorScreen.h"
#include "LevelEditor.h"

#include <IOManager.h>

GuiEditor::GuiEditor(EditorScreen* parent) :
m_parent(parent)
{
}

GuiEditor::~GuiEditor()
{
	std::vector<CEGUI::ListboxTextItem*>().swap(m_lbiMusic);
	std::vector<CEGUI::ListboxTextItem*>().swap(m_lbiSound);
	std::vector<CEGUI::ListboxTextItem*>().swap(m_lbiTexture);
	std::vector<CEGUI::ListboxTextItem*>().swap(m_lbiEntities);
}

void GuiEditor::load(std::vector<CEGUI::ListboxTextItem*>& list, const char* path)
{
	// Get all directory entries
	std::vector<NS2::DirEntry> entries;
	NS2::IOManager::getDirectoryEntries(path, entries);

	list.push_back(new CEGUI::ListboxTextItem("Nothing"));
	// Add all files to list box
	for (auto& e : entries) {
		// Don't add directories
		if (!e.isDirectory) {
			// Remove directory substring + /
			e.path.erase(0, std::string(path).size() + 1);
			list.push_back(new CEGUI::ListboxTextItem(e.path));
		}
	}
}

void GuiEditor::loadEntityType(std::vector<CEGUI::ListboxTextItem*>& list)
{
	list.push_back(new CEGUI::ListboxTextItem("BAG"));
	list.push_back(new CEGUI::ListboxTextItem("AMMO"));
	list.push_back(new CEGUI::ListboxTextItem("FLASHLIGHT"));
	list.push_back(new CEGUI::ListboxTextItem("GUN"));
	list.push_back(new CEGUI::ListboxTextItem("KEY"));
	list.push_back(new CEGUI::ListboxTextItem("MEDOC"));
}

void GuiEditor::load(CEGUI::Combobox* cbb, std::vector<CEGUI::ListboxTextItem*>& list)
{
	for (CEGUI::ListboxTextItem* item : list)
	{
		cbb->addItem(item);
	}

}

void GuiEditor::clear(CEGUI::Combobox* cbb, std::vector<CEGUI::ListboxTextItem*>& list)
{
	for (CEGUI::ListboxTextItem* item : list)
	{
		cbb->removeItem(item);
	}
}

void GuiEditor::updateColours(float r, float g, float b, float a)
{
	m_sRed->setCurrentValue(r);
	m_sGreen->setCurrentValue(g);
	m_sBlue->setCurrentValue(b);
	m_sAlpha->setCurrentValue(a);

	m_colour.set(r, g, b, a);
	m_coloursRect.setColours(m_colour);
	m_imgColor->setProperty("ClientAreaColour", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(m_coloursRect));
}
void GuiEditor::setRed(float r)
{
	m_colour.setRed(r);
	m_coloursRect.setColours(m_colour);
	m_imgColor->setProperty("ClientAreaColour", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(m_coloursRect));
}
void GuiEditor::setGreen(float g)
{
	m_colour.setGreen(g);
	m_coloursRect.setColours(m_colour);
	m_imgColor->setProperty("ClientAreaColour", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(m_coloursRect));
}
void GuiEditor::setBlue(float b)
{
	m_colour.setBlue(b);
	m_coloursRect.setColours(m_colour);
	m_imgColor->setProperty("ClientAreaColour", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(m_coloursRect));
}
void GuiEditor::setAlpha(float a)
{
	m_colour.setAlpha(a);
	m_coloursRect.setColours(m_colour);
	m_imgColor->setProperty("ClientAreaColour", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(m_coloursRect));
}

bool GuiEditor::setSelectedFile(CEGUI::Combobox* cbb, const std::string& dir, std::string& path)
{
	path.erase(0, std::string(dir).size() + 1);
	if (cbb->getSelectedItem() != nullptr){
		if (path == cbb->getSelectedItem()->getText())
			return false;
	}
	else{
		cbb->setItemSelectState((size_t)0, true);
		return false;
	}

	for (unsigned int i = 0; i < cbb->getItemCount(); i++)
	{
		if (cbb->getListboxItemFromIndex(i)->getText() == path)
		{
			cbb->setItemSelectState(i, true);
			return true;
		}
	}

	cbb->setItemSelectState((size_t)0, true);
	return false;
}

void GuiEditor::initWithGui(NS2::Gui& gui)
{
	m_window = gui.createWindow("NS_editor_2.layout");
	m_dialog.init(gui, m_window);
	CEGUI::TabControl* winTabControl = static_cast<CEGUI::TabControl*>(m_window->getChild("ctn_custom"));
	CEGUI::Window* coffer = gui.createWindow("NS_coffer.layout");
	CEGUI::Window* door = gui.createWindow("NS_door.layout");
	CEGUI::Window* bag = gui.createWindow("NS_bag.layout");
	CEGUI::Window* ammo = gui.createWindow("NS_ammo.layout");
	CEGUI::Window* flashlight = gui.createWindow("NS_flashlight.layout");
	CEGUI::Window* gun = gui.createWindow("NS_gun.layout");
	CEGUI::Window* human = gui.createWindow("NS_human.layout");
	CEGUI::Window* key = gui.createWindow("NS_key.layout");
	CEGUI::Window* level = gui.createWindow("NS_level.layout");
	CEGUI::Window* light = gui.createWindow("NS_light.layout");
	CEGUI::Window* medicine = gui.createWindow("NS_medicine.layout");
	CEGUI::Window* player = gui.createWindow("NS_player.layout");
	CEGUI::Window* tile = gui.createWindow("NS_tile.layout");
	CEGUI::Window* zombie = gui.createWindow("NS_zombie.layout");
	CEGUI::Window* node = gui.createWindow("NS_node.layout"); 

	winTabControl->addTab(level);
	winTabControl->addTab(tile);
	winTabControl->addTab(node);
	winTabControl->addTab(light);
	winTabControl->addTab(player);
	winTabControl->addTab(human);
	winTabControl->addTab(zombie);
	winTabControl->addTab(coffer);
	winTabControl->addTab(door);
	winTabControl->addTab(gun);
	winTabControl->addTab(key);
	winTabControl->addTab(bag);
	winTabControl->addTab(ammo);
	winTabControl->addTab(flashlight);
	winTabControl->addTab(medicine);

	m_dialog.getWindow()->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&GuiEditor::onDialogClose, this));
	level->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onLevelActive, this));
	node->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onNodeActive, this));
	player->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onPlayerActive, this));
	door->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onDoorActive, this));
	gun->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onGunActive, this));
	human->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onHumanActive, this));
	key->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onKeyActive, this));
	bag->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onBagActive, this));
	ammo->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onAmmoActive, this));
	flashlight->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onFlashlightActive, this));
	light->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onLightActive, this));
	medicine->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onMedicineActive, this));
	tile->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onTileActive, this));
	zombie->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onZombieActive, this));
	coffer->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&GuiEditor::onCofferActive, this));

	load(m_lbiTexture, "Assets");
	load(m_lbiMusic, "Musics");
	load(m_lbiSound, "Sounds");
	loadEntityType(m_lbiEntities);

	/* Entities widgets initialization */
	m_sAlpha = static_cast<CEGUI::Slider*> (m_window->getChild("ctn_entities/gbx_color/sld_alpha"));
	m_sAlpha->subscribeEvent(CEGUI::Slider::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onColorPickerAlphaChanged, this));

	m_sRed = static_cast<CEGUI::Slider*> (m_window->getChild("ctn_entities/gbx_color/sld_red"));
	m_sRed->subscribeEvent(CEGUI::Slider::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onColorPickerRedChanged, this));

	m_sGreen = static_cast<CEGUI::Slider*> (m_window->getChild("ctn_entities/gbx_color/sld_green"));
	m_sGreen->subscribeEvent(CEGUI::Slider::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onColorPickerGreenChanged, this));

	m_sBlue = static_cast<CEGUI::Slider*> (m_window->getChild("ctn_entities/gbx_color/sld_blue"));
	m_sBlue->subscribeEvent(CEGUI::Slider::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onColorPickerBlueChanged, this));

	m_imgColor = static_cast<CEGUI::FrameWindow*> (m_window->getChild("ctn_entities/gbx_color/img_color"));
	m_imgColor->setProperty("ClientAreaColour", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(m_coloursRect));

	m_cbTexture = static_cast<CEGUI::Combobox*> (m_window->getChild("ctn_entities/gbx_color/cbx_texture"));
	m_cbTexture->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::onTextureSelected, this));
	load(m_cbTexture, m_lbiTexture);

	m_spTexDimX = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/gbx_color/spn_texDimX"));
	m_spTexDimX->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onTexDimXValueChange, this));

	m_spTexDimY = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/gbx_color/spn_texDimY"));
	m_spTexDimY->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onTexDimYValueChange, this));

	m_spWidth = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/spn_width"));
	m_spWidth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onWidthValueChange, this));

	m_spHeight = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/spn_height"));
	m_spHeight->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onHeightValueChange, this));

	m_spAnimSpeed = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/spn_animSpeed"));
	m_spAnimSpeed->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onAnimSpeedValueChange, this));

	m_spDirX = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/spn_dirX"));
	m_spDirX->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onDirXValueChange, this));

	m_spDirY = static_cast<CEGUI::Spinner*> (m_window->getChild("ctn_entities/spn_dirY"));
	m_spDirY->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onDirYValueChange, this));

	m_ebName = static_cast<CEGUI::Editbox*> (m_window->getChild("ctn_entities/ebx_name"));
	m_ebName->subscribeEvent(CEGUI::Editbox::EventTextChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onNameValueChange, this));

	m_rbDynamic = static_cast<CEGUI::RadioButton*> (m_window->getChild("ctn_entities/gbx_entityState/rbt_dynamic"));
	m_rbDynamic->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onStateMouseClick, this));

	m_rbStatic = static_cast<CEGUI::RadioButton*> (m_window->getChild("ctn_entities/gbx_entityState/rbt_static"));
	m_rbStatic->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onStateMouseClick, this));

	m_rbCircle = static_cast<CEGUI::RadioButton*> (m_window->getChild("ctn_entities/gbx_shape/rbt_circle"));
	m_rbCircle->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onShapeMouseClick, this));

	m_rbBox = static_cast<CEGUI::RadioButton*> (m_window->getChild("ctn_entities/gbx_shape/rbt_box"));
	m_rbBox->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onShapeMouseClick, this));

	m_rbPlace = static_cast<CEGUI::RadioButton*> (m_window->getChild("ctn_action/gbx_action/rbt_place"));
	m_rbPlace->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onActionClick, this));

	m_rbSelect = static_cast<CEGUI::RadioButton*> (m_window->getChild("ctn_action/gbx_action/rbt_select"));
	m_rbSelect->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onActionClick, this));

	m_tgDebug = static_cast<CEGUI::ToggleButton*> (m_window->getChild("ctn_action/chb_debug"));
	m_tgDebug->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onDebugToggleClick, this));

	m_playMusic = static_cast<CEGUI::ToggleButton*> (m_window->getChild("ctn_action/chb_playMusic"));
	m_playMusic->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onPlayMusicToggleClick, this));

	m_btUpdate = static_cast<CEGUI::PushButton*> (m_window->getChild("ctn_action/btn_update"));
	m_btUpdate->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GuiEditor::onUpdateClick, this));

	m_btSave = static_cast<CEGUI::PushButton*> (m_window->getChild("ctn_action/btn_save"));
	m_btSave->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GuiEditor::onSaveClick, this));

	m_btQuit = static_cast<CEGUI::PushButton*> (m_window->getChild("ctn_action/btn_quit"));
	m_btQuit->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GuiEditor::onQuitClick, this));

	m_btLoad = static_cast<CEGUI::PushButton*> (m_window->getChild("ctn_action/btn_load"));
	m_btLoad->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GuiEditor::onLoadClick, this));

	/* Coffer widgets initialization */
	m_tgCofferDestru = static_cast<CEGUI::ToggleButton*> (coffer->getChild("gbx_members/chb_destructible"));
	m_tgCofferDestru->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onCofferDestruToggleClick, this));

	m_rbCofferThis = static_cast<CEGUI::RadioButton*> (coffer->getChild("gbx_selected/rbt_this"));
	m_rbCofferThis->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_CofferSelected_MouseClick, this));

	m_rbCofferEntity = static_cast<CEGUI::RadioButton*> (coffer->getChild("gbx_selected/rbt_entity"));
	m_rbCofferEntity->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_CofferSelected_MouseClick, this));

	m_spCofferHealth = static_cast<CEGUI::Spinner*> (coffer->getChild("gbx_members/spn_health"));
	m_spCofferHealth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_CofferHealth_ValueChange, this));

	m_cbCofferShootSound = static_cast<CEGUI::Combobox*> (coffer->getChild("gbx_members/cbx_shootSound"));
	m_cbCofferShootSound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_CofferShootSound_Selected, this));
	load(m_cbCofferShootSound, m_lbiSound);

	m_cbCofferCrashSound = static_cast<CEGUI::Combobox*> (coffer->getChild("gbx_members/cbx_crashSound"));
	m_cbCofferCrashSound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_CofferCrashSound_Selected, this));
	load(m_cbCofferCrashSound, m_lbiSound);

	m_cbCofferEntities = static_cast<CEGUI::Combobox*> (coffer->getChild("gbx_selected/cbx_entities"));
	m_cbCofferEntities->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_CofferEntities_Selected, this));
	load(m_cbCofferEntities, m_lbiEntities);

	/* Door widgets initialization */
	m_ebKeyname = static_cast<CEGUI::Editbox*> (door->getChild("gbx_members/eb_keyname"));
	m_ebKeyname->subscribeEvent(CEGUI::Editbox::EventTextChanged,
		CEGUI::Event::Subscriber(&GuiEditor::onKeyNameValueChange, this));

	m_tgDoorOpened = static_cast<CEGUI::ToggleButton*> (door->getChild("gbx_members/chb_opened"));
	m_tgDoorOpened->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_DoorOpened_ToggleClick, this));

	m_cbDoorSound = static_cast<CEGUI::Combobox*> (door->getChild("gbx_members/cbx_sound"));
	m_cbDoorSound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_DoorSound_Selected, this));
	load(m_cbDoorSound, m_lbiSound);

	/* Bag widgets initialization */
	m_spCols = static_cast<CEGUI::Spinner*> (bag->getChild("sp_cols"));
	m_spCols->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_BagCols_ValueChange, this));

	m_spRows = static_cast<CEGUI::Spinner*> (bag->getChild("sp_rows"));
	m_spRows->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_BagRows_ValueChange, this));

	/* Flashlight widgets initialization */
	m_tgOn = static_cast<CEGUI::ToggleButton*> (flashlight->getChild("chb_on"));
	m_tgOn->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_FlashlightOn_ToggleClick, this));

	m_spLow = static_cast<CEGUI::Spinner*> (flashlight->getChild("sp_low"));
	m_spLow->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_FlashlighLow_ValueChange, this));

	m_spDuration = static_cast<CEGUI::Spinner*> (flashlight->getChild("sp_duration"));
	m_spDuration->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_FlashlighDuration_ValueChange, this));

	/* Ammo widgets initialization */
	m_spAmount = static_cast<CEGUI::Spinner*> (ammo->getChild("sp_amount"));
	m_spAmount->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_AmmoAmount_ValueChange, this));

	m_cbAmmoGuntype = static_cast<CEGUI::Combobox*> (ammo->getChild("cb_guntype"));
	m_cbAmmoGuntype->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_AmmoGuntype_Selected, this));

	m_cbAmmoGuntype->addItem(new CEGUI::ListboxTextItem("RIFLE", (size_t)GunType::RIFLE));
	m_cbAmmoGuntype->addItem(new CEGUI::ListboxTextItem("SHOTGUN", (size_t)GunType::SHOTGUN));
	m_cbAmmoGuntype->addItem(new CEGUI::ListboxTextItem("HANDGUN", (size_t)GunType::HANDGUN));

	/* Gun widgets initialization */
	m_spGunLoader = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_loader"));
	m_spGunLoader->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunLoader_ValueChange, this));

	m_spGunRate = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_rate"));
	m_spGunRate->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunRate_ValueChange, this));

	m_spGunSpread = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_spread"));
	m_spGunSpread->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunSpread_ValueChange, this));

	m_spGunSpeed = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_speed"));
	m_spGunSpeed->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunSpeed_ValueChange, this));

	m_spGunBps = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_bps"));
	m_spGunBps->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunBsp_ValueChange, this));

	m_spGunDamage = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_damage"));
	m_spGunDamage->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunDamage_ValueChange, this));

	m_spGunBulletWidth = static_cast<CEGUI::Spinner*> (gun->getChild("gbx_members/spn_bulletWidth"));
	m_spGunBulletWidth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunBulletWidth_ValueChange, this));

	m_cbGunSound = static_cast<CEGUI::Combobox*> (gun->getChild("gbx_members/cbx_sound"));
	m_cbGunSound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunSound_Selected, this));
	load(m_cbGunSound, m_lbiSound);

	m_cbGunType = static_cast<CEGUI::Combobox*> (gun->getChild("gbx_members/cbx_type"));
	m_cbGunType->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_GunType_Selected, this));
	m_cbGunType->addItem(new CEGUI::ListboxTextItem("RIFLE", (size_t)GunType::RIFLE));
	m_cbGunType->addItem(new CEGUI::ListboxTextItem("SHOTGUN", (size_t)GunType::SHOTGUN));
	m_cbGunType->addItem(new CEGUI::ListboxTextItem("HANDGUN", (size_t)GunType::HANDGUN));

	/* Human widgets initialization */
	m_spHumanSpeed = static_cast<CEGUI::Spinner*> (human->getChild("gbx_members/spn_speed"));
	m_spHumanSpeed->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_HumanSpeed_ValueChange, this));

	m_spHumanHealth = static_cast<CEGUI::Spinner*> (human->getChild("gbx_members/spn_health"));
	m_spHumanHealth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_HumanHealth_ValueChange, this));

	/* Key widgets initialization */
	m_cbKeySound = static_cast<CEGUI::Combobox*> (key->getChild("gbx_members/cbx_sound"));
	m_cbKeySound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_KeySound_Selected, this));
	load(m_cbKeySound, m_lbiSound);

	/* Medicine widgets initialization */
	m_spMedicineHealth = static_cast<CEGUI::Spinner*> (medicine->getChild("gbx_members/spn_health"));
	m_spMedicineHealth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_MedicineHealth_ValueChange, this));

	m_cbMedicineSound = static_cast<CEGUI::Combobox*> (medicine->getChild("gbx_members/cbx_sound"));
	m_cbMedicineSound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_MedicineSound_Selected, this));
	load(m_cbMedicineSound, m_lbiSound);

	/* Tile widgets initialization */
	m_tgTileCrossable = static_cast<CEGUI::ToggleButton*> (tile->getChild("gbx_members/chb_opened"));
	m_tgTileCrossable->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_TileCrossable_ToggleClick, this));

	m_spTilePosX = static_cast<CEGUI::Spinner*> (tile->getChild("gbx_members/spn_posX"));
	m_spTilePosX->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_TilePosX_ValueChange, this));

	m_spTilePosY = static_cast<CEGUI::Spinner*> (tile->getChild("gbx_members/spn_posY"));
	m_spTilePosY->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_TilePosY_ValueChange, this));

	/* Zombie widgets initialization */
	m_spZombieSpeed = static_cast<CEGUI::Spinner*> (zombie->getChild("gbx_members/spn_speed"));
	m_spZombieSpeed->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_ZombieSpeed_ValueChange, this));

	m_spZombieHealth = static_cast<CEGUI::Spinner*> (zombie->getChild("gbx_members/spn_health"));
	m_spZombieHealth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_ZombieHealth_ValueChange, this));

	m_spZombieDamage = static_cast<CEGUI::Spinner*> (zombie->getChild("gbx_members/spn_damage"));
	m_spZombieDamage->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_ZombieDamage_ValueChange, this));

	/* Player widgets initialization */
	m_spPlayerNbkeys = static_cast<CEGUI::Spinner*> (player->getChild("gbx_members/spn_keys"));
	m_spPlayerNbkeys->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerNbkeys_ValueChange, this));

	m_spPlayerSpeed = static_cast<CEGUI::Spinner*> (player->getChild("gbx_members/spn_speed"));
	m_spPlayerSpeed->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSpeed_ValueChange, this));

	m_spPlayerHealth = static_cast<CEGUI::Spinner*> (player->getChild("gbx_members/spn_health"));
	m_spPlayerHealth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerHealth_ValueChange, this));

	m_tgPlayerFlashLight = static_cast<CEGUI::ToggleButton*> (player->getChild("gbx_members/chb_lightOn"));
	m_tgPlayerFlashLight->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerFlashLight_ToggleClick, this));

	m_tgPlayerGun1 = static_cast<CEGUI::ToggleButton*> (player->getChild("gbx_members/chb_gun1"));
	m_tgPlayerGun1->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerGun1_ToggleClick, this));

	m_tgPlayerGun2 = static_cast<CEGUI::ToggleButton*> (player->getChild("gbx_members/chb_gun2"));
	m_tgPlayerGun2->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerGun2_ToggleClick, this));

	m_tgPlayerSourceLight = static_cast<CEGUI::ToggleButton*> (player->getChild("gbx_members/chb_sourceLight"));
	m_tgPlayerSourceLight->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSourceLight_ToggleClick, this));

	m_tgPlayerAroundLight = static_cast<CEGUI::ToggleButton*> (player->getChild("gbx_members/chb_aroundLight"));
	m_tgPlayerAroundLight->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerAroundLight_ToggleClick, this));

	m_tgPlayerDiffLight = static_cast<CEGUI::ToggleButton*> (player->getChild("gbx_members/chb_diffLight"));
	m_tgPlayerDiffLight->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerDiffLight_ToggleClick, this));

	m_rbPlayerThis = static_cast<CEGUI::RadioButton*> (player->getChild("gbx_selected/rbt_this"));
	m_rbPlayerThis->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSelected_MouseClick, this));

	m_rbPlayerGun1 = static_cast<CEGUI::RadioButton*> (player->getChild("gbx_selected/rbt_gun1"));
	m_rbPlayerGun1->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSelected_MouseClick, this));

	m_rbPlayerGun2 = static_cast<CEGUI::RadioButton*> (player->getChild("gbx_selected/rbt_gun2"));
	m_rbPlayerGun2->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSelected_MouseClick, this));

	m_rbPlayerSourceLight = static_cast<CEGUI::RadioButton*> (player->getChild("gbx_selected/rbt_sourceLight"));
	m_rbPlayerSourceLight->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSelected_MouseClick, this));

	m_rbPlayerAroundLight = static_cast<CEGUI::RadioButton*> (player->getChild("gbx_selected/rbt_aroundLight"));
	m_rbPlayerAroundLight->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSelected_MouseClick, this));

	m_rbPlayerDiffLight = static_cast<CEGUI::RadioButton*> (player->getChild("gbx_selected/rbt_diffLight"));
	m_rbPlayerDiffLight->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_PlayerSelected_MouseClick, this));

	/* Light widgets initialization */
	m_rbLightPointLight = static_cast<CEGUI::RadioButton*> (light->getChild("gbx_members/rbt_pointLight"));
	m_rbLightPointLight->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LightPointSpot_MouseClick, this));

	m_rbLightSpotLight = static_cast<CEGUI::RadioButton*> (light->getChild("gbx_members/rbt_spotLight"));
	m_rbLightSpotLight->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LightPointSpot_MouseClick, this));

	m_tgLightOn = static_cast<CEGUI::ToggleButton*> (light->getChild("gbx_members/chb_on"));
	m_tgLightOn->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LightOn_ToggleClick, this));

	m_tgLightBroken = static_cast<CEGUI::ToggleButton*> (light->getChild("gbx_members/chb_broken"));
	m_tgLightBroken->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LightBroken_ToggleClick, this));

	m_spLightPercent = static_cast<CEGUI::Spinner*> (light->getChild("gbx_members/spn_percent"));
	m_spLightPercent->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LightPercent_ValueChange, this));

	m_cbLightSound = static_cast<CEGUI::Combobox*> (light->getChild("gbx_members/cbx_sound"));
	m_cbLightSound->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_LightSound_Selected, this));
	load(m_cbLightSound, m_lbiSound);

	/* Graph Node widgets initialization */
	m_spNodeWidth = static_cast<CEGUI::Spinner*> (node->getChild("gbx_members/spn_width"));
	m_spNodeWidth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_NodeWidth_ValueChange, this));

	m_spNodeHeight = static_cast<CEGUI::Spinner*> (node->getChild("gbx_members/spn_height"));
	m_spNodeHeight->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_NodeHeight_ValueChange, this));

	m_rbNodeThis = static_cast<CEGUI::RadioButton*> (node->getChild("gbx_selected/rbt_this"));
	m_rbNodeThis->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_NodeSelected_MouseClick, this));

	m_rbNodeCenter = static_cast<CEGUI::RadioButton*> (node->getChild("gbx_selected/rbt_center"));
	m_rbNodeCenter->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_NodeSelected_MouseClick, this));

	m_rbNodeEdge = static_cast<CEGUI::RadioButton*> (node->getChild("gbx_selected/rbt_edge"));
	m_rbNodeEdge->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_NodeSelected_MouseClick, this));

	/* Level widgets initialization */
	m_spLevelWidth = static_cast<CEGUI::Spinner*> (level->getChild("gbx_members/spn_width"));
	m_spLevelWidth->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LevelWidth_ValueChange, this));

	m_spLevelHeight = static_cast<CEGUI::Spinner*> (level->getChild("gbx_members/spn_height"));
	m_spLevelHeight->subscribeEvent(CEGUI::Spinner::EventValueChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LevelHeight_ValueChange, this));

	m_cbLevelMusic = static_cast<CEGUI::Combobox*> (level->getChild("gbx_members/cbx_music"));
	m_cbLevelMusic->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
		CEGUI::Event::Subscriber(&GuiEditor::on_LevelMusic_Selected, this));
	load(m_cbLevelMusic, m_lbiMusic);

	m_tgLevelShot = static_cast<CEGUI::ToggleButton*> (level->getChild("gbx_members/chb_screen"));
	m_tgLevelShot->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&GuiEditor::on_LevelShot_ToggleClick, this));
}

void GuiEditor::update()
{
	LevelEditor& level = m_parent->m_level;

	if (level.objectMode == ObjectMode::LEVEL)
	{
		m_spLevelWidth->setCurrentValue(level.width);
		m_spLevelHeight->setCurrentValue(level.height);
		m_ebName->setText(level.name);

		float red = level.ambientColor.r / 255.0f;
		float green = level.ambientColor.g / 255.0f;
		float blue = level.ambientColor.b / 255.0f;
		float alpha = level.ambientColor.a / 255.0f;

		updateColours(red, green, blue, alpha);
		std::string str = level.music.getFilePath();
		setSelectedFile(m_cbLevelMusic, "Musics", str);
		if (!NS2::Music::playing() && m_playMusic->isSelected()) {
			level.music.play();
		}
	}
	else if (level.objectMode == ObjectMode::NODE)
	{
		m_rbNodeThis->setSelected(level.nodeMode == NodeMode::NODE);
		m_rbNodeCenter->setSelected(level.nodeMode == NodeMode::CENTER);
		m_rbNodeEdge->setSelected(level.nodeMode == NodeMode::EDGE);

		if (level.nodeMode == NodeMode::EDGE) {
			m_spNodeWidth->setCurrentValue(level.edge.getWidth());
		}
		else {
			GraphNode* n = level.getSelectedNode();
			if (n != nullptr){
				m_spNodeWidth->setCurrentValue(n->getShape().destRect.z);
				m_spNodeHeight->setCurrentValue(n->getShape().destRect.w);
				bool b = n->getShape().type == ShapeType::BOX;
				m_rbBox->setSelected(b);
				m_rbCircle->setSelected(!b);
			}
		}
	}
	else if (level.objectMode == ObjectMode::TILE)
	{
		m_tgTileCrossable->setSelected(level.tile.isCrossable());

		std::string str = level.tile.getTexture().filePath;
		setSelectedFile(m_cbTexture, "Assets", str);

		float red = level.tile.getColor().r / 255.0f;
		float green = level.tile.getColor().g / 255.0f;
		float blue = level.tile.getColor().b / 255.0f;
		float alpha = level.tile.getColor().a / 255.0f;

		updateColours(red, green, blue, alpha);
	}
	else if (level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = level.getSelectedLight();
		if (l != nullptr)
		{
			m_rbLightPointLight->setSelected(l->type == LightType::POINT_LIGHT);
			m_rbLightSpotLight->setSelected(l->type == LightType::SPOT_LIGHT);
			m_spWidth->setCurrentValue(l->destRect.z);
			m_spDirX->setCurrentValue(l->direction.x);
			m_spDirY->setCurrentValue(l->direction.y);
			m_tgLightOn->setSelected(l->on);
			m_tgLightBroken->setSelected(l->brokenDown);
			m_spLightPercent->setCurrentValue(l->percent);

			std::string str = l->effect.getFilePath();
			setSelectedFile(m_cbLightSound, "Sounds", str);
			str = l->texture.filePath;
			setSelectedFile(m_cbTexture, "Assets", str);

			float red = l->color.r / 255.0f;
			float green = l->color.g / 255.0f;
			float blue = l->color.b / 255.0f;
			float alpha = l->color.a / 255.0f;

			updateColours(red, green, blue, alpha);
		}
	}
	else
	{
		Entity* e = nullptr;

		if (level.objectMode == ObjectMode::PLAYER)
		{
			e = &level.player;
			m_spPlayerSpeed->setCurrentValue(level.player.getSpeed());
			m_spPlayerHealth->setCurrentValue(level.player.getHealth());

			m_tgPlayerGun1->setSelected(level.playerGun1 != NO_SELECT);
			m_tgPlayerGun2->setSelected(level.playerGun2 != NO_SELECT);
			m_tgPlayerSourceLight->setSelected(level.playerSrc != NO_SELECT);
			m_tgPlayerAroundLight->setSelected(level.playerAround != NO_SELECT);
			m_tgPlayerDiffLight->setSelected(level.playerDiff != NO_SELECT);
		}
		else if (level.objectMode == ObjectMode::HUMAN)
		{
			Human* h = level.getSelectedHuman();
			if (h != nullptr)
			{
				m_spHumanSpeed->setCurrentValue(h->getSpeed());
				m_spHumanHealth->setCurrentValue(h->getHealth());
				e = h;
			}
		}
		else if (level.objectMode == ObjectMode::ZOMBIE)
		{
			Zombie* z = level.getSelectedZombie();
			if (z != nullptr)
			{
				m_spZombieSpeed->setCurrentValue(z->getSpeed());
				m_spZombieHealth->setCurrentValue(z->getHealth());
				m_spZombieDamage->setCurrentValue(z->getDamage());
				e = z;
			}
		}
		else if (level.objectMode == ObjectMode::COFFER)
		{
			Coffer* c = level.getSelectedCoffer();
			if (c != nullptr)
			{
				m_spCofferHealth->setCurrentValue(c->getHealth());
				m_tgCofferDestru->setSelected(c->isDestructible());

				std::string str = c->getSound().getFilePath();
				setSelectedFile(m_cbCofferCrashSound, "Sounds", str);
				str = c->getShootSound().getFilePath();
				setSelectedFile(m_cbCofferShootSound, "Sounds", str);

				if (level.selectionMode == SelectionMode::SELECT)
				{
					if (level.cofferMode == CofferMode::COFFER)
						m_rbCofferThis->setSelected(true);
					else
					{
						m_rbCofferEntity->setSelected(true);
						if (level.cofferMode == CofferMode::BAG)
							m_cbCofferEntities->setItemSelectState((unsigned int)0, true);
						else if(level.cofferMode == CofferMode::AMMO)
							m_cbCofferEntities->setItemSelectState((unsigned int)1, true);
						else if(level.cofferMode == CofferMode::FLASHLIGHT)
							m_cbCofferEntities->setItemSelectState((unsigned int)2, true);
						else if (level.cofferMode == CofferMode::GUN)
							m_cbCofferEntities->setItemSelectState((unsigned int)3, true);
						else if (level.cofferMode == CofferMode::KEY)
							m_cbCofferEntities->setItemSelectState((unsigned int)4, true);
						else if (level.cofferMode == CofferMode::MEDICINE)
							m_cbCofferEntities->setItemSelectState((unsigned int)5, true);
					}
				}

				e = c;
			}
		}
		else if (level.objectMode == ObjectMode::KEY)
		{
			Key* k = level.getSelectedKey();
			if (k != nullptr)
			{
				std::string str = k->getSound().getFilePath();
				setSelectedFile(m_cbKeySound, "Sounds", str);
				e = k;
			}
		}
		else if (level.objectMode == ObjectMode::AMMO)
		{
			Ammo* a = level.getSelectedAmmo();
			if (a != nullptr)
			{
				m_spAmount->setCurrentValue(a->getAmount());
				m_cbAmmoGuntype->setItemSelectState((size_t)a->getGunType(), true);
				e = a;
			}
		}
		else if (level.objectMode == ObjectMode::BAG)
		{
			Bag* b = level.getSelectedBag();
			if (b != nullptr)
			{
				m_spCols->setCurrentValue(b->getColumns());
				m_spRows->setCurrentValue(b->getRows());
				e = b;
			}
		}
		else if (level.objectMode == ObjectMode::FLASHLIGHT)
		{
			Flashlight* f = level.getSelectedFlashlight();
			if (f != nullptr)
			{
				m_spLow->setCurrentValue(f->getLow());
				m_spDuration->setCurrentValue(f->getDuration());
				m_tgOn->setSelected(f->isOn());
				e = f;
			}
		}
		else if (level.objectMode == ObjectMode::DOOR)
		{
			Door* d = level.getSelectedDoor();
			if (d != nullptr)
			{
				m_ebKeyname->setText(d->getKeyName());
				m_tgDoorOpened->setSelected(d->isOpened());
				std::string str = d->getSound().getFilePath();
				setSelectedFile(m_cbDoorSound, "Sounds", str);
				e = d;
			}
		}
		else if (level.objectMode == ObjectMode::GUN)
		{
			Gun* g = level.getSelectedGun();
			if (g != nullptr)
			{
				m_cbGunType->setItemSelectState((size_t)g->getGunType(), true);
				m_spGunLoader->setCurrentValue(g->getLoader());
				m_spGunRate->setCurrentValue(g->getRate());
				m_spGunSpread->setCurrentValue(g->getSpread());
				m_spGunSpeed->setCurrentValue(g->getSpeed());
				m_spGunBps->setCurrentValue(g->getBPS());
				m_spGunBulletWidth->setCurrentValue(g->getBulletSize());
				m_spGunDamage->setCurrentValue(g->getDamage());
				std::string str = g->getSound().getFilePath();
				setSelectedFile(m_cbGunSound, "Sounds", str);
				e = g;
			}
		}
		else if (level.objectMode == ObjectMode::MEDICINE)
		{
			Medicine* m = level.getSelectedMedoc();
			if (m != nullptr)
			{
				m_spMedicineHealth->setCurrentValue(m->getHealth());
				std::string str = m->getSound().getFilePath();
				setSelectedFile(m_cbMedicineSound, "Sounds", str);
				e = m;
			}
		}
		if (e != nullptr)
		{
			bool state = e->getState() == EntityState::DYNAMIC;
			bool shape = e->getShape().type == ShapeType::CIRCLE;

			m_rbDynamic->setSelected(state);
			m_rbStatic->setSelected(!state);
			m_rbCircle->setSelected(shape);
			m_rbBox->setSelected(!shape);
			m_spWidth->setCurrentValue(e->getShape().destRect.z);
			m_spHeight->setCurrentValue(e->getShape().destRect.w);
			m_spAnimSpeed->setCurrentValue(e->getAnimSpeed());
			m_spDirX->setCurrentValue(e->getDirection().x);
			m_spDirY->setCurrentValue(e->getDirection().y);
			m_spTexDimX->setCurrentValue(e->getTexture().dims.x);
			m_spTexDimY->setCurrentValue(e->getTexture().dims.y);
			m_ebName->setText(e->getName());

			std::string str = e->getTexture().texture.filePath;
			setSelectedFile(m_cbTexture, "Assets", str);

			float red = e->getColor().r / 255.0f;
			float green = e->getColor().g / 255.0f;
			float blue = e->getColor().b / 255.0f;
			float alpha = e->getColor().a / 255.0f;

			updateColours(red, green, blue, alpha);
		}
	}
}

/************************************************************************/
/* Event Handlers                                                       */
/************************************************************************/
/* All Entities Event handlers */
bool GuiEditor::onLevelActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::LEVEL;
	update();
	return true;
}
bool GuiEditor::onNodeActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::NODE;
	update();
	return true;
}
bool GuiEditor::onTileActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::TILE;
	update();
	return true;
}
bool GuiEditor::onLightActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::LIGHT;
	update();
	return true;
}
bool GuiEditor::onPlayerActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::PLAYER;
	update();
	return true;
}
bool GuiEditor::onZombieActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::ZOMBIE;
	update();
	return true;
}
bool GuiEditor::onHumanActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::HUMAN;
	update();
	return true;
}
bool GuiEditor::onCofferActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::COFFER;
	update();
	return true;
}
bool GuiEditor::onDoorActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::DOOR;
	update();
	return true;
}
bool GuiEditor::onGunActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::GUN;
	update();
	return true;
}
bool GuiEditor::onKeyActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::KEY;
	update();
	return true;
}
bool GuiEditor::onBagActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::BAG;
	update();
	return true;
}
bool GuiEditor::onAmmoActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::AMMO;
	update();
	return true;
}
bool GuiEditor::onFlashlightActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::FLASHLIGHT;
	update();
	return true;
}
bool GuiEditor::onMedicineActive(const CEGUI::EventArgs& e)
{
	m_parent->m_level.objectMode = ObjectMode::MEDICINE;
	update();
	return true;
}
bool GuiEditor::onColorPickerRedChanged(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	float red = m_sRed->getCurrentValue();
	setRed(red);

	if (level.objectMode == ObjectMode::LEVEL)
	{
		level.ambientColor.r = (GLuint)(red * 255);
	}
	else if (level.objectMode == ObjectMode::TILE)
	{
		level.tile.getColor().r = (GLuint)(red * 255);
	}
	else if (level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = level.getSelectedLight();
		if (l != nullptr)
		{
			l->color.r = (GLuint)(red * 255);
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();

		if (e != nullptr)
		{
			e->getColor().r = (GLuint)(red * 255);
		}
	}
	return true;
}
bool GuiEditor::onColorPickerGreenChanged(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	float green = m_sGreen->getCurrentValue();
	setGreen(green);

	if (level.objectMode == ObjectMode::LEVEL)
	{
		level.ambientColor.g = (GLuint)(green * 255);
	}
	if (level.objectMode == ObjectMode::TILE)
	{
		level.tile.getColor().g = (GLuint)(green * 255);
	}
	else if (level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = level.getSelectedLight();
		if (l != nullptr)
		{
			l->color.g = (GLuint)(green * 255);
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();

		if (e != nullptr)
		{
			e->getColor().g = (GLuint)(green * 255);
		}
	}
	return true;
}
bool GuiEditor::onColorPickerBlueChanged(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	float blue = m_sBlue->getCurrentValue();
	setBlue(blue);

	if (level.objectMode == ObjectMode::LEVEL)
	{
		level.ambientColor.b = (GLuint)(blue * 255);
	}
	if (level.objectMode == ObjectMode::TILE)
	{
		level.tile.getColor().b = (GLuint)(blue * 255);
	}
	else if (level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = level.getSelectedLight();
		if (l != nullptr)
		{
			l->color.b = (GLuint)(blue * 255);
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();

		if (e != nullptr)
		{
			e->getColor().b = (GLuint)(blue * 255);
		}
	}
	return true;
}
bool GuiEditor::onColorPickerAlphaChanged(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	float alpha = m_sAlpha->getCurrentValue();
	setAlpha(alpha);

	if (level.objectMode == ObjectMode::LEVEL)
	{
		level.ambientColor.a = (GLuint)(alpha * 255);
	}
	if (level.objectMode == ObjectMode::TILE)
	{
		level.tile.getColor().a = (GLuint)(alpha * 255);
	}
	else if (level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = level.getSelectedLight();
		if (l != nullptr)
		{
			l->color.a = (GLuint)(alpha * 255);
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();

		if (e != nullptr)
		{
			e->getColor().a = (GLuint)(alpha * 255);
		}
	}
	return true;
}
bool GuiEditor::onNameValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;

	if (level.objectMode == ObjectMode::LEVEL)
	{
		level.name = m_ebName->getText().c_str();
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();

		if (e != nullptr)
		{
			e->setName(m_ebName->getText().c_str());
		}
	}
	return true;
}
bool GuiEditor::onKeyNameValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Door* d = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		d = &level.door;
	else if (level.indexDoor != NO_SELECT)
		d = &level.doors[level.indexDoor];
	if (d != nullptr)
	{
		d->setKeyName(m_ebKeyname->getText().c_str());
	}

	return true;
}
bool GuiEditor::onStateMouseClick(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();

	if (e != nullptr)
	{
		e->setState((m_rbDynamic->isSelected() ? EntityState::DYNAMIC : EntityState::STATIC));
	}

	return true;
}
bool GuiEditor::onShapeMouseClick(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();

	if (e != nullptr)
	{
		e->getShape().type = (m_rbCircle->isSelected() ? ShapeType::CIRCLE : ShapeType::BOX);
	}
	else
	{
		GraphNode* n = m_parent->m_level.getSelectedNode();

		if (n != nullptr)
			n->getShape().type = (m_rbCircle->isSelected() ? ShapeType::CIRCLE : ShapeType::BOX);
	}
	return true;
}
bool GuiEditor::onActionClick(const CEGUI::EventArgs& e)
{
	m_parent->m_level.selectionMode = (m_rbPlace->isSelected()) ? SelectionMode::PLACE : SelectionMode::SELECT;
	return true;
}
bool GuiEditor::onTexDimXValueChange(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();
	if (e != nullptr)
	{
		e->getTexture().dims.x = m_spTexDimX->getCurrentValue();
	}
	return true;
}
bool GuiEditor::onTexDimYValueChange(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();
	if (e != nullptr)
	{
		e->getTexture().dims.y = m_spTexDimY->getCurrentValue();
	}
	return true;
}
bool GuiEditor::onDirXValueChange(const CEGUI::EventArgs& ev)
{
	if (m_parent->m_level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = m_parent->m_level.getSelectedLight();
		if (l != nullptr)
		{
			l->direction.x = m_spDirX->getCurrentValue();
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();
		if (e != nullptr)
		{
			e->getDirection().x = m_spDirX->getCurrentValue();
		}
	}

	return true;
}
bool GuiEditor::onDirYValueChange(const CEGUI::EventArgs& ev)
{
	if (m_parent->m_level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = m_parent->m_level.getSelectedLight();
		if (l != nullptr)
		{
			l->direction.y = m_spDirY->getCurrentValue();
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();
		if (e != nullptr)
		{
			e->getDirection().y = m_spDirY->getCurrentValue();
		}
	}
	return true;
}
bool GuiEditor::onWidthValueChange(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();
	if (e != nullptr)
	{
		e->getShape().destRect.z = m_spWidth->getCurrentValue();
	}
	return true;
}
bool GuiEditor::onHeightValueChange(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();
	if (e != nullptr)
	{
		e->getShape().destRect.w = m_spHeight->getCurrentValue();
	}
	return true;
}
bool GuiEditor::onAnimSpeedValueChange(const CEGUI::EventArgs& ev)
{
	Entity* e = m_parent->m_level.getSelectedEntity();
	if (e != nullptr)
	{
		e->setAnimSpeed(m_spAnimSpeed->getCurrentValue());
	}
	return true;
}
bool GuiEditor::onDebugToggleClick(const CEGUI::EventArgs& e)
{
	m_parent->m_renderer.setDebug(m_tgDebug->isSelected());
	return true;
}
bool GuiEditor::onPlayMusicToggleClick(const CEGUI::EventArgs& e)
{
	if (!m_playMusic->isSelected())
		m_parent->m_level.music.stop();
	else
		m_parent->m_level.music.play();

	return true;
}
bool GuiEditor::onTextureSelected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	std::string str = std::string("Assets/") + m_cbTexture->getSelectedItem()->getText().c_str();

	if (level.objectMode == ObjectMode::TILE)
	{
		level.tile.setTexture(NS2::ResourceManager::getTexture(str));
	}
	else if (level.objectMode == ObjectMode::LIGHT)
	{
		Light* l = level.getSelectedLight();
		if (l != nullptr)
		{
			l->texture = NS2::ResourceManager::getTexture(str);
		}
	}
	else
	{
		Entity* e = m_parent->m_level.getSelectedEntity();
		if (e != nullptr)
		{
			e->getTexture().texture = NS2::ResourceManager::getTexture(str);
		}
	}

	return true;
}
bool GuiEditor::onQuitClick(const CEGUI::EventArgs& e)
{
	m_dialog.initQuestion(1, "QUit", "Would you like to leave Level Editor ?");
	return true;
}
bool GuiEditor::onSaveClick(const CEGUI::EventArgs& e)
{
	std::string error;

	if (!m_parent->saveLevel(error))
		m_dialog.initConfirm(2, "Save Error", error);
	else
		m_dialog.initConfirm(2, "Save Completed", "Level has been saved successfully");

	return true;
}
bool GuiEditor::onLoadClick(const CEGUI::EventArgs& e)
{
	m_lbilevel.clear();
	load(m_lbilevel, "Levels");
	m_dialog.initSelect(3, m_lbilevel, "Load Level", "Select Level to load");

	return true;
}
bool GuiEditor::onDialogClose(const CEGUI::EventArgs& e)
{
	if (m_dialog.getId() == 1)
	{
		if (m_dialog.isOk()) {
			m_parent->setGameState(NS2::ScreenState::CHANGE_PREVIOUS);
		}
	}
	if (m_dialog.getId() == 3)
	{
		if (m_dialog.isOk()) {
			std::string error;
			std::string fileName = m_dialog.getTextValue();
			if (fileName.empty())
				m_dialog.initConfirm(4, "Load Error", "Incorrect file : " + fileName);
			else if (!m_parent->loadLevel(fileName, error))
				m_dialog.initConfirm(4, "Load Error", error);
			else {
				m_dialog.initConfirm(4, "Load Completed", "Level has been loaded successfully");
				static_cast<CEGUI::TabControl*>(m_window->getChild("ctn_custom"))->setSelectedTabAtIndex(0);
				m_rbPlace->setSelected(true);
			}
		}
	}
	return true;
}
bool GuiEditor::onUpdateClick(const CEGUI::EventArgs& e)
{
	m_parent->m_level.resizeTiles();
	m_btUpdate->disable();
	return true;
}

/* Coffer Event handlers */
bool GuiEditor::onCofferDestruToggleClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Coffer* c = level.getSelectedCoffer();
	if (c != nullptr)
	{
		c->setDestructible(m_tgCofferDestru->isSelected());
	}
	return true;
}
bool GuiEditor::on_CofferHealth_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Coffer* c = level.getSelectedCoffer();
	if (c != nullptr)
	{
		c->setHealth(m_spCofferHealth->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_CofferShootSound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Coffer* c = level.getSelectedCoffer();
	if (c != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbCofferShootSound->getSelectedItem()->getText().c_str();
		c->setShootSound(NS2::ResourceManager::getSound(str));
		c->getShootSound().play();
	}
	return true;
}
bool GuiEditor::on_CofferCrashSound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Coffer* c = level.getSelectedCoffer();
	if (c != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbCofferCrashSound->getSelectedItem()->getText().c_str();
		c->setSound(NS2::ResourceManager::getSound(str));
		c->getSound().play();
	}
	return true;
}
bool GuiEditor::on_CofferEntities_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;

	if (level.selectionMode == SelectionMode::SELECT && level.indexCoffer != NO_SELECT)
	{
		if (m_rbCofferThis->isSelected())
			level.cofferMode = CofferMode::COFFER;
		else if (m_cbCofferEntities->getSelectedItem() != nullptr)
		{
			if (m_cbCofferEntities->getSelectedItem()->getText() == "BAG")
				level.cofferMode = CofferMode::BAG;
			else if(m_cbCofferEntities->getSelectedItem()->getText() == "AMMO")
				level.cofferMode = CofferMode::AMMO;
			else if(m_cbCofferEntities->getSelectedItem()->getText() == "FLASHLIGHT")
				level.cofferMode = CofferMode::FLASHLIGHT;
			else if (m_cbCofferEntities->getSelectedItem()->getText() == "GUN")
				level.cofferMode = CofferMode::GUN;
			else if (m_cbCofferEntities->getSelectedItem()->getText() == "KEY")
				level.cofferMode = CofferMode::KEY;
			else if (m_cbCofferEntities->getSelectedItem()->getText() == "MEDOC")
				level.cofferMode = CofferMode::MEDICINE;
		}
	}
	return true;
}
bool GuiEditor::on_CofferSelected_MouseClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;

	if (level.selectionMode == SelectionMode::SELECT && level.indexCoffer != NO_SELECT)
	{
		if (m_rbCofferThis->isSelected())
			level.cofferMode = CofferMode::COFFER;
		else if (m_cbCofferEntities->getSelectedItem() != nullptr)
		{
			if (m_cbCofferEntities->getSelectedItem()->getText() == "BAG")
				level.cofferMode = CofferMode::BAG;
			else if(m_cbCofferEntities->getSelectedItem()->getText() == "AMMO")
				level.cofferMode = CofferMode::AMMO;
			else if(m_cbCofferEntities->getSelectedItem()->getText() == "FLASHLIGHT")
				level.cofferMode = CofferMode::FLASHLIGHT;
			else if (m_cbCofferEntities->getSelectedItem()->getText() == "GUN")
				level.cofferMode = CofferMode::GUN;
			else if (m_cbCofferEntities->getSelectedItem()->getText() == "KEY")
				level.cofferMode = CofferMode::KEY;
			else if (m_cbCofferEntities->getSelectedItem()->getText() == "MEDOC")
				level.cofferMode = CofferMode::MEDICINE;
		}
	}
	return true;
}

/* Door Event handlers */
bool GuiEditor::on_DoorOpened_ToggleClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Door* d = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		d = &level.door;
	else if (level.indexDoor != NO_SELECT)
		d = &level.doors[level.indexDoor];
	if (d != nullptr)
	{
		d->setOpened(m_tgDoorOpened->isSelected());
	}
	return true;
}
bool GuiEditor::on_DoorSound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Door* d = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		d = &level.door;
	else if (level.indexDoor != NO_SELECT)
		d = &level.doors[level.indexDoor];
	if (d != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbDoorSound->getSelectedItem()->getText().c_str();
		d->setSound(NS2::ResourceManager::getSound(str));
		d->getSound().play();
	}
	return true;
}

/* Gun Event handlers */
bool GuiEditor::on_GunLoader_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setLoader(m_spGunLoader->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunRate_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setRate(m_spGunRate->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunSpread_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setSpread(m_spGunSpread->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunSpeed_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setSpeed(m_spGunSpeed->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunBsp_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setBPS(m_spGunBps->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunDamage_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setDamage(m_spGunDamage->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunBulletWidth_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setBulletSize(m_spGunBulletWidth->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_GunType_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		g->setGunType((GunType)m_cbGunType->getSelectedItem()->getID());
	}
	return true;
}
bool GuiEditor::on_GunSound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Gun* g = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		g = &level.gun;
	else if (level.indexGun != NO_SELECT)
		g = &level.guns[level.indexGun];
	if (g != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbGunSound->getSelectedItem()->getText().c_str();
		g->setSound(NS2::ResourceManager::getSound(str));
		g->getSound().play();
	}
	return true;
}


/* Key Event handlers */
bool GuiEditor::on_KeySound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Key* k = level.getSelectedKey();
	if (k != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbKeySound->getSelectedItem()->getText().c_str();
		k->setSound(NS2::ResourceManager::getSound(str));
		k->getSound().play();
	}
	return true;
}

/* Flashlight Event handlers */
bool GuiEditor::on_FlashlightOn_ToggleClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Flashlight* d = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		d = &level.flashlight;
	else if (level.indexFlashlight != NO_SELECT)
		d = &level.flashlights[level.indexFlashlight];
	if (d != nullptr)
	{
		d->setOn(m_tgOn->isSelected());
	}
	return true;
}
bool GuiEditor::on_FlashlighLow_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Flashlight* d = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		d = &level.flashlight;
	else if (level.indexFlashlight != NO_SELECT)
		d = &level.flashlights[level.indexFlashlight];
	if (d != nullptr)
	{
		d->setLow(m_spLow->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_FlashlighDuration_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Flashlight* d = nullptr;
	if (level.selectionMode == SelectionMode::PLACE)
		d = &level.flashlight;
	else if (level.indexFlashlight != NO_SELECT)
		d = &level.flashlights[level.indexFlashlight];
	if (d != nullptr)
	{
		d->setDuration(m_spDuration->getCurrentValue());
	}
	return true;
}

/* Bag Event handlers */
bool GuiEditor::on_BagCols_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Bag* b = level.getSelectedBag();
	if (b != nullptr)
	{
		b->setColumns(m_spCols->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_BagRows_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Bag* b = level.getSelectedBag();
	if (b != nullptr)
	{
		b->setRows(m_spRows->getCurrentValue());
	}
	return true;
}

/* Ammo Event handlers */
bool GuiEditor::on_AmmoAmount_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Ammo* b = level.getSelectedAmmo();
	if (b != nullptr)
	{
		b->setAmount(m_spAmount->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_AmmoGuntype_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Ammo* m = level.getSelectedAmmo();
	if (m != nullptr)
	{
		m->setGunType((GunType)m_cbAmmoGuntype->getSelectedItem()->getID());
	}
	return true;
}

/* Medicine Event handlers */
bool GuiEditor::on_MedicineHealth_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Medicine* m = level.getSelectedMedoc();
	if (m != nullptr)
	{
		m->setHealth(m_spMedicineHealth->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_MedicineSound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Medicine* m = level.getSelectedMedoc();
	if (m != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbMedicineSound->getSelectedItem()->getText().c_str();
		m->setSound(NS2::ResourceManager::getSound(str));
		m->getSound().play();
	}
	return true;
}

/* Human Event handlers */
bool GuiEditor::on_HumanSpeed_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Human* h = level.getSelectedHuman();
	if (h != nullptr)
	{
		h->setSpeed(m_spHumanSpeed->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_HumanHealth_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Human* h = level.getSelectedHuman();
	if (h != nullptr)
	{
		h->setHealth(m_spHumanHealth->getCurrentValue());
	}
	return true;
	return true;
}

/* Zombie Event handlers */
bool GuiEditor::on_ZombieSpeed_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Zombie* z = level.getSelectedZombie();
	if (z != nullptr)
	{
		z->setSpeed(m_spZombieSpeed->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_ZombieHealth_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Zombie* z = level.getSelectedZombie();
	if (z != nullptr)
	{
		z->setHealth(m_spZombieHealth->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_ZombieDamage_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Zombie* z = level.getSelectedZombie();
	if (z != nullptr)
	{
		z->setDamage(m_spZombieDamage->getCurrentValue());
	}
	return true;
}

/* Light Event handlers */
bool GuiEditor::on_LightPointSpot_MouseClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Light* l = level.getSelectedLight();
	if (l != nullptr)
	{
		l->setType(m_rbLightPointLight->isSelected() ? LightType::POINT_LIGHT : LightType::SPOT_LIGHT);
	}
	return true;
}
bool GuiEditor::on_LightOn_ToggleClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Light* l = level.getSelectedLight();
	if (l != nullptr)
	{
		l->on = (m_tgLightOn->isSelected());
	}
	return true;
}
bool GuiEditor::on_LightBroken_ToggleClick(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Light* l = level.getSelectedLight();
	if (l != nullptr)
	{
		l->brokenDown = (m_tgLightBroken->isSelected());
	}
	return true;
}
bool GuiEditor::on_LightPercent_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Light* l = level.getSelectedLight();
	if (l != nullptr)
	{
		l->percent = (m_spLightPercent->getCurrentValue());
	}
	return true;
}
bool GuiEditor::on_LightSound_Selected(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	Light* l = level.getSelectedLight();
	if (l != nullptr)
	{
		std::string str = std::string("Sounds/") + m_cbLightSound->getSelectedItem()->getText().c_str();
		l->effect = NS2::ResourceManager::getSound(str);
		l->effect.play();
	}
	return true;
}

/* Player Event handlers */
bool GuiEditor::on_PlayerNbkeys_ValueChange(const CEGUI::EventArgs& e)
{
	return true;
}
bool GuiEditor::on_PlayerHealth_ValueChange(const CEGUI::EventArgs& e)
{
	m_parent->m_level.player.setHealth(m_spPlayerHealth->getCurrentValue());
	return true;
}
bool GuiEditor::on_PlayerSpeed_ValueChange(const CEGUI::EventArgs& e)
{
	m_parent->m_level.player.setSpeed(m_spPlayerSpeed->getCurrentValue());
	return true;
}
bool GuiEditor::on_PlayerFlashLight_ToggleClick(const CEGUI::EventArgs& e)
{
	return true;
}
bool GuiEditor::on_PlayerGun1_ToggleClick(const CEGUI::EventArgs& e)
{
	if (!m_tgPlayerGun1->isSelected())
		m_parent->m_level.playerGun1 = NO_SELECT;
	else if (m_parent->m_level.playerGun1 == NO_SELECT)
		m_tgPlayerGun1->setSelected(false);

	return true;
}
bool GuiEditor::on_PlayerGun2_ToggleClick(const CEGUI::EventArgs& e)
{
	if (!m_tgPlayerGun2->isSelected())
		m_parent->m_level.playerGun2 = NO_SELECT;
	else if (m_parent->m_level.playerGun2 == NO_SELECT)
		m_tgPlayerGun2->setSelected(false);
	return true;
}
bool GuiEditor::on_PlayerSourceLight_ToggleClick(const CEGUI::EventArgs& e)
{
	if (!m_tgPlayerSourceLight->isSelected())
		m_parent->m_level.playerSrc = NO_SELECT;
	else if (m_parent->m_level.playerSrc == NO_SELECT)
		m_tgPlayerSourceLight->setSelected(false);
	return true;
}
bool GuiEditor::on_PlayerAroundLight_ToggleClick(const CEGUI::EventArgs& e)
{
	if (!m_tgPlayerAroundLight->isSelected())
		m_parent->m_level.playerAround = NO_SELECT;
	else if (m_parent->m_level.playerAround == NO_SELECT)
		m_tgPlayerAroundLight->setSelected(false);
	return true;
}
bool GuiEditor::on_PlayerDiffLight_ToggleClick(const CEGUI::EventArgs& e)
{
	if (!m_tgPlayerDiffLight->isSelected())
		m_parent->m_level.playerDiff = NO_SELECT;
	else if (m_parent->m_level.playerDiff == NO_SELECT)
		m_tgPlayerDiffLight->setSelected(false);
	return true;
}

bool GuiEditor::on_PlayerSelected_MouseClick(const CEGUI::EventArgs& e)
{
	if (m_rbPlayerGun1->isSelected())
		m_parent->m_level.playerMode = PlayerMode::GUN1;
	else if (m_rbPlayerGun2->isSelected())
		m_parent->m_level.playerMode = PlayerMode::GUN2;
	else if (m_rbPlayerSourceLight->isSelected())
		m_parent->m_level.playerMode = PlayerMode::SRC_LIGHT;
	else if (m_rbPlayerAroundLight->isSelected())
		m_parent->m_level.playerMode = PlayerMode::AROUND_LIGHT;
	else if (m_rbPlayerDiffLight->isSelected())
		m_parent->m_level.playerMode = PlayerMode::DIFF_LIGHT;
	else
		m_parent->m_level.playerMode = PlayerMode::PLAYER;
	return true;
}

/* Tile Event handlers */
bool GuiEditor::on_TileCrossable_ToggleClick(const CEGUI::EventArgs& e)
{
	m_parent->m_level.tile.setCrossable(m_tgTileCrossable->isSelected());
	return true;
}
bool GuiEditor::on_TilePosX_ValueChange(const CEGUI::EventArgs& e){ return true; }
bool GuiEditor::on_TilePosY_ValueChange(const CEGUI::EventArgs& e){ return true; }

/* GraphNode Event handlers */
bool GuiEditor::on_NodeWidth_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	if (level.nodeMode == NodeMode::EDGE){
		level.edge.setWidth(m_spNodeWidth->getCurrentValue());
	}
	else {
		GraphNode* n = level.getSelectedNode();
		if (n != nullptr)
			n->getShape().destRect.z = m_spNodeWidth->getCurrentValue();
	}

	return true;
}
bool GuiEditor::on_NodeHeight_ValueChange(const CEGUI::EventArgs& e)
{
	LevelEditor& level = m_parent->m_level;
	GraphNode* n = level.getSelectedNode();

	if (n != nullptr)
		n->getShape().destRect.w = m_spNodeHeight->getCurrentValue();

	return true;
}
bool GuiEditor::on_NodeSelected_MouseClick(const CEGUI::EventArgs& e)
{
	if (m_rbNodeThis->isSelected()) {
		m_parent->m_level.nodeMode = NodeMode::NODE;
		m_parent->m_level.edge.reset();
	}
	else if (m_rbNodeCenter->isSelected()) {
		m_parent->m_level.nodeMode = NodeMode::CENTER;
		m_parent->m_level.edge.reset();
	}
	else if (m_rbNodeEdge->isSelected())
		m_parent->m_level.nodeMode = NodeMode::EDGE;
	return true;
}

/* Level Event handlers */
bool GuiEditor::on_LevelWidth_ValueChange(const CEGUI::EventArgs& e)
{
	m_parent->m_level.width = m_spLevelWidth->getCurrentValue();
	m_btUpdate->enable();
	return true;
}
bool GuiEditor::on_LevelHeight_ValueChange(const CEGUI::EventArgs& e)
{
	m_parent->m_level.height = m_spLevelHeight->getCurrentValue();
	m_btUpdate->enable();
	return true;
}
bool GuiEditor::on_LevelMusic_Selected(const CEGUI::EventArgs& e)
{
	std::string str = std::string("Musics/") + m_cbLevelMusic->getSelectedItem()->getText().c_str();
	m_parent->m_level.music.stop();
	m_parent->m_level.music = NS2::ResourceManager::getMusic(str);
	if (m_playMusic->isSelected())
		m_parent->m_level.music.play();
	return true;
}

bool GuiEditor::on_LevelShot_ToggleClick(const CEGUI::EventArgs& e)
{
	m_parent->m_level.onScreenShoot = m_tgLevelShot->isSelected();
	m_parent->m_level.screenRect.x = 0.0f;
	m_parent->m_level.screenRect.y = 0.0f;
	m_parent->m_level.screenRect.z = 0.0f;
	m_parent->m_level.screenRect.w = 0.0f;
	return true;
}



