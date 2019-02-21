#pragma once

#include "GuiDialog.h"

#include <IGuiWindow.h>

class EditorScreen;

class GuiEditor : public NS2::IGuiWindow
{
public:
	GuiEditor(EditorScreen* parent);

	~GuiEditor();

	virtual void initWithGui(NS2::Gui& gui) override;
	void update();
	void load(std::vector<CEGUI::ListboxTextItem*>& list, const char* path);
	void loadEntityType(std::vector<CEGUI::ListboxTextItem*>& list);
	void load(CEGUI::Combobox* cbb, std::vector<CEGUI::ListboxTextItem*>& list);
	void clear(CEGUI::Combobox* cbb, std::vector<CEGUI::ListboxTextItem*>& list);
	void updateColours(float r, float g, float b, float a);
	void setRed(float r);
	void setGreen(float g);
	void setBlue(float b);
	void setAlpha(float a);
	bool setSelectedFile(CEGUI::Combobox* cbb, const std::string& dir, std::string& path);

	/************************************************************************/
	/* Event Handlers                                                       */
	/************************************************************************/
	/* All Entities Event handlers */
	bool onColorPickerRedChanged(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChanged(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChanged(const CEGUI::EventArgs& e);
	bool onColorPickerAlphaChanged(const CEGUI::EventArgs& e);
	bool onStateMouseClick(const CEGUI::EventArgs& e);
	bool onShapeMouseClick(const CEGUI::EventArgs& e);
	bool onNameValueChange(const CEGUI::EventArgs& e);
	bool onKeyNameValueChange(const CEGUI::EventArgs& e);
	bool onActionClick(const CEGUI::EventArgs& e);
	bool onTexDimXValueChange(const CEGUI::EventArgs& e);
	bool onTexDimYValueChange(const CEGUI::EventArgs& e);
	bool onDirXValueChange(const CEGUI::EventArgs& e);
	bool onDirYValueChange(const CEGUI::EventArgs& e);
	bool onWidthValueChange(const CEGUI::EventArgs& e);
	bool onHeightValueChange(const CEGUI::EventArgs& e);
	bool onAnimSpeedValueChange(const CEGUI::EventArgs& e);
	bool onDebugToggleClick(const CEGUI::EventArgs& e);
	bool onPlayMusicToggleClick(const CEGUI::EventArgs& e);
	bool onTextureSelected(const CEGUI::EventArgs& e);
	bool onQuitClick(const CEGUI::EventArgs& e);
	bool onUpdateClick(const CEGUI::EventArgs& e);
	bool onSaveClick(const CEGUI::EventArgs& e);
	bool onLoadClick(const CEGUI::EventArgs& e);
	bool onLevelActive(const CEGUI::EventArgs& e);
	bool onNodeActive(const CEGUI::EventArgs& e);
	bool onTileActive(const CEGUI::EventArgs& e);
	bool onLightActive(const CEGUI::EventArgs& e);
	bool onPlayerActive(const CEGUI::EventArgs& e);
	bool onZombieActive(const CEGUI::EventArgs& e);
	bool onHumanActive(const CEGUI::EventArgs& e);
	bool onCofferActive(const CEGUI::EventArgs& e);
	bool onDoorActive(const CEGUI::EventArgs& e);
	bool onGunActive(const CEGUI::EventArgs& e);
	bool onKeyActive(const CEGUI::EventArgs& e);
	bool onBagActive(const CEGUI::EventArgs& e);
	bool onAmmoActive(const CEGUI::EventArgs& e);
	bool onFlashlightActive(const CEGUI::EventArgs& e);
	bool onMedicineActive(const CEGUI::EventArgs& e);
	bool onDialogClose(const CEGUI::EventArgs& e);
	bool onCofferDestruToggleClick(const CEGUI::EventArgs& e);

	/* Coffer Event handlers */
	bool on_CofferHealth_ValueChange(const CEGUI::EventArgs& e);
	bool on_CofferShootSound_Selected(const CEGUI::EventArgs& e);
	bool on_CofferCrashSound_Selected(const CEGUI::EventArgs& e);
	bool on_CofferEntities_Selected(const CEGUI::EventArgs& e);
	bool on_CofferSelected_MouseClick(const CEGUI::EventArgs& e);

	/* Door Event handlers */
	bool on_DoorOpened_ToggleClick(const CEGUI::EventArgs& e);
	bool on_DoorSound_Selected(const CEGUI::EventArgs& e);

	/* Gun Event handlers */
	bool on_GunLoader_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunRate_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunSpread_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunSpeed_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunBsp_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunDamage_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunBulletWidth_ValueChange(const CEGUI::EventArgs& e);
	bool on_GunType_Selected(const CEGUI::EventArgs& e);
	bool on_GunSound_Selected(const CEGUI::EventArgs& e);

	/* Key Event handlers */
	bool on_KeySound_Selected(const CEGUI::EventArgs& e);

	/* Bag Event handlers */
	bool on_BagCols_ValueChange(const CEGUI::EventArgs& e);
	bool on_BagRows_ValueChange(const CEGUI::EventArgs& e);

	/* Ammo Event handlers */
	bool on_AmmoAmount_ValueChange(const CEGUI::EventArgs& e);
	bool on_AmmoGuntype_Selected(const CEGUI::EventArgs& e);

	/* Flashlight Event handlers */
	bool on_FlashlightOn_ToggleClick(const CEGUI::EventArgs& e);
	bool on_FlashlighLow_ValueChange(const CEGUI::EventArgs& e);
	bool on_FlashlighDuration_ValueChange(const CEGUI::EventArgs& e);

	/* Medicine Event handlers */
	bool on_MedicineHealth_ValueChange(const CEGUI::EventArgs& e);
	bool on_MedicineSound_Selected(const CEGUI::EventArgs& e);

	/* Human Event handlers */
	bool on_HumanSpeed_ValueChange(const CEGUI::EventArgs& e);
	bool on_HumanHealth_ValueChange(const CEGUI::EventArgs& e);

	/* Zombie Event handlers */
	bool on_ZombieSpeed_ValueChange(const CEGUI::EventArgs& e);
	bool on_ZombieHealth_ValueChange(const CEGUI::EventArgs& e);
	bool on_ZombieDamage_ValueChange(const CEGUI::EventArgs& e);

	/* Light Event handlers */
	bool on_LightPointSpot_MouseClick(const CEGUI::EventArgs& e);
	bool on_LightOn_ToggleClick(const CEGUI::EventArgs& e);
	bool on_LightBroken_ToggleClick(const CEGUI::EventArgs& e);
	bool on_LightPercent_ValueChange(const CEGUI::EventArgs& e);
	bool on_LightSound_Selected(const CEGUI::EventArgs& e);

	/* Player Event handlers */
	bool on_PlayerNbkeys_ValueChange(const CEGUI::EventArgs& e);
	bool on_PlayerHealth_ValueChange(const CEGUI::EventArgs& e);
	bool on_PlayerSpeed_ValueChange(const CEGUI::EventArgs& e);
	bool on_PlayerFlashLight_ToggleClick(const CEGUI::EventArgs& e);
	bool on_PlayerGun1_ToggleClick(const CEGUI::EventArgs& e);
	bool on_PlayerGun2_ToggleClick(const CEGUI::EventArgs& e);
	bool on_PlayerSourceLight_ToggleClick(const CEGUI::EventArgs& e);
	bool on_PlayerAroundLight_ToggleClick(const CEGUI::EventArgs& e);
	bool on_PlayerDiffLight_ToggleClick(const CEGUI::EventArgs& e);
	bool on_PlayerSelected_MouseClick(const CEGUI::EventArgs& e);

	/* Tile Event handlers */
	bool on_TileCrossable_ToggleClick(const CEGUI::EventArgs& e);
	bool on_TilePosX_ValueChange(const CEGUI::EventArgs& e);
	bool on_TilePosY_ValueChange(const CEGUI::EventArgs& e);

	/* Node Event handlers */
	bool on_NodeWidth_ValueChange(const CEGUI::EventArgs& e);
	bool on_NodeHeight_ValueChange(const CEGUI::EventArgs& e);
	bool on_NodeSelected_MouseClick(const CEGUI::EventArgs& e);

	/* Level Event handlers */
	bool on_LevelWidth_ValueChange(const CEGUI::EventArgs& e);
	bool on_LevelHeight_ValueChange(const CEGUI::EventArgs& e);
	bool on_LevelMusic_Selected(const CEGUI::EventArgs& e);
	bool on_LevelShot_ToggleClick(const CEGUI::EventArgs& e);

private:
	EditorScreen* m_parent = nullptr;
	GuiDialog m_dialog;
	CEGUI::Colour m_colour;
	CEGUI::ColourRect m_coloursRect;

	std::vector<CEGUI::ListboxTextItem*> m_lbiEntities;
	std::vector<CEGUI::ListboxTextItem*> m_lbiMusic;
	std::vector<CEGUI::ListboxTextItem*> m_lbiSound;
	std::vector<CEGUI::ListboxTextItem*> m_lbiTexture;
	std::vector<CEGUI::ListboxTextItem*> m_lbilevel;

	/* All Entities widgets */
	CEGUI::Window* m_imgColor;
	CEGUI::Slider* m_sAlpha = nullptr;
	CEGUI::Slider* m_sRed = nullptr;
	CEGUI::Slider* m_sGreen = nullptr;
	CEGUI::Slider* m_sBlue = nullptr;

	CEGUI::RadioButton* m_rbStatic = nullptr;
	CEGUI::RadioButton* m_rbDynamic = nullptr;
	CEGUI::RadioButton* m_rbCircle = nullptr;
	CEGUI::RadioButton* m_rbBox = nullptr;
	CEGUI::RadioButton* m_rbSelect = nullptr;
	CEGUI::RadioButton* m_rbPlace = nullptr;

	CEGUI::ToggleButton* m_tgDebug = nullptr;
	CEGUI::ToggleButton* m_playMusic = nullptr;
	CEGUI::Spinner* m_spDirX = nullptr;
	CEGUI::Spinner* m_spDirY = nullptr;
	CEGUI::Spinner* m_spAnimSpeed = nullptr;
	CEGUI::Spinner* m_spWidth = nullptr;
	CEGUI::Spinner* m_spHeight = nullptr;
	CEGUI::Spinner* m_spTexDimX = nullptr;
	CEGUI::Spinner* m_spTexDimY = nullptr;

	CEGUI::Editbox* m_ebName = nullptr;
	
	CEGUI::Combobox* m_cbTexture = nullptr;

	CEGUI::PushButton* m_btUpdate = nullptr;
	CEGUI::PushButton* m_btSave = nullptr;
	CEGUI::PushButton* m_btQuit = nullptr;
	CEGUI::PushButton* m_btLoad = nullptr;

	/* Coffer widgets */
	CEGUI::ToggleButton* m_tgCofferDestru = nullptr;
	CEGUI::RadioButton* m_rbCofferEntity = nullptr;
	CEGUI::RadioButton* m_rbCofferThis = nullptr;
	CEGUI::Spinner* m_spCofferHealth = nullptr;
	CEGUI::Combobox* m_cbCofferShootSound = nullptr;
	CEGUI::Combobox* m_cbCofferCrashSound = nullptr;
	CEGUI::Combobox* m_cbCofferEntities = nullptr;

	/* Door widgets */
	CEGUI::ToggleButton* m_tgDoorOpened = nullptr;
	CEGUI::Combobox* m_cbDoorSound = nullptr;
	CEGUI::Editbox* m_ebKeyname = nullptr;

	/* Bag widgets */
	CEGUI::Spinner* m_spCols = nullptr;
	CEGUI::Spinner* m_spRows = nullptr;

	/* Ammo widgets */
	CEGUI::Spinner* m_spAmount = nullptr;
	CEGUI::Combobox* m_cbAmmoGuntype = nullptr;

	/* Flashlight widgets */
	CEGUI::Spinner* m_spLow = nullptr;
	CEGUI::Spinner* m_spDuration = nullptr;
	CEGUI::ToggleButton* m_tgOn = nullptr;

	/* Medicine widgets */
	CEGUI::Spinner* m_spMedicineHealth = nullptr;
	CEGUI::Combobox* m_cbMedicineSound = nullptr;

	/* Key widgets */
	CEGUI::Combobox* m_cbKeySound = nullptr;

	/* Gun widgets */
	CEGUI::Spinner* m_spGunLoader = nullptr;
	CEGUI::Spinner* m_spGunRate = nullptr;
	CEGUI::Spinner* m_spGunSpread = nullptr;
	CEGUI::Spinner* m_spGunSpeed = nullptr;
	CEGUI::Spinner* m_spGunBps = nullptr;
	CEGUI::Spinner* m_spGunDamage = nullptr;
	CEGUI::Spinner* m_spGunBulletWidth = nullptr;
	CEGUI::Combobox* m_cbGunSound = nullptr;
	CEGUI::Combobox* m_cbGunType = nullptr;

	/* Human widgets */
	CEGUI::Spinner* m_spHumanSpeed = nullptr;
	CEGUI::Spinner* m_spHumanHealth = nullptr;

	/* Zombie widgets */
	CEGUI::Spinner* m_spZombieSpeed = nullptr;
	CEGUI::Spinner* m_spZombieHealth = nullptr;
	CEGUI::Spinner* m_spZombieDamage = nullptr;

	/* Player widgets */
	CEGUI::Spinner* m_spPlayerNbkeys = nullptr;
	CEGUI::Spinner* m_spPlayerHealth = nullptr;
	CEGUI::Spinner* m_spPlayerSpeed = nullptr;
	CEGUI::ToggleButton* m_tgPlayerFlashLight = nullptr;
	CEGUI::ToggleButton* m_tgPlayerGun1 = nullptr;
	CEGUI::ToggleButton* m_tgPlayerGun2 = nullptr;
	CEGUI::ToggleButton* m_tgPlayerSourceLight = nullptr;
	CEGUI::ToggleButton* m_tgPlayerAroundLight = nullptr;
	CEGUI::ToggleButton* m_tgPlayerDiffLight = nullptr;
	CEGUI::RadioButton* m_rbPlayerThis = nullptr;
	CEGUI::RadioButton* m_rbPlayerGun1 = nullptr;
	CEGUI::RadioButton* m_rbPlayerGun2 = nullptr;
	CEGUI::RadioButton* m_rbPlayerSourceLight = nullptr;
	CEGUI::RadioButton* m_rbPlayerAroundLight = nullptr;
	CEGUI::RadioButton* m_rbPlayerDiffLight = nullptr;

	/* Light widgets */
	CEGUI::RadioButton* m_rbLightPointLight = nullptr;
	CEGUI::RadioButton* m_rbLightSpotLight = nullptr;
	CEGUI::ToggleButton* m_tgLightOn = nullptr;
	CEGUI::ToggleButton* m_tgLightBroken = nullptr;
	CEGUI::Spinner* m_spLightPercent = nullptr;
	CEGUI::Combobox* m_cbLightSound = nullptr;

	/* Tile widgets */
	CEGUI::ToggleButton* m_tgTileCrossable = nullptr;
	CEGUI::Spinner* m_spTilePosX = nullptr;
	CEGUI::Spinner* m_spTilePosY = nullptr;

	/* Graph node widgets */
	CEGUI::Spinner* m_spNodeWidth = nullptr;
	CEGUI::Spinner* m_spNodeHeight = nullptr;
	CEGUI::RadioButton* m_rbNodeThis = nullptr;
	CEGUI::RadioButton* m_rbNodeCenter = nullptr;
	CEGUI::RadioButton* m_rbNodeEdge = nullptr;

	/* Level widgets */
	CEGUI::Spinner* m_spLevelWidth = nullptr;
	CEGUI::Spinner* m_spLevelHeight = nullptr;
	CEGUI::Combobox* m_cbLevelMusic = nullptr;
	CEGUI::ToggleButton* m_tgLevelShot = nullptr;
};

