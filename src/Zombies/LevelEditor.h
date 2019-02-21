#pragma once

#include "GraphEdge.h"
#include "GraphNode.h"
#include "Grid.h"
#include "Tile.h"
#include "Light.h"

#include "Entity.h"

#include "Shape.h"

#include "Door.h"
#include "Medicine.h"
#include "Gun.h"
#include "Coffer.h"
#include "Key.h"
#include "Flashlight.h"
#include "Ammo.h"

#include "Zombie.h"
#include "Human.h"
#include "Player.h"

#include <unordered_map>
#include <ResourceManager.h>
#include <AudioEngine.h>

enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

enum class ObjectMode {
	LEVEL, LIGHT, TILE, PLAYER, ZOMBIE, HUMAN, COFFER, GUN, KEY, DOOR, MEDICINE, NODE, BAG, AMMO, FLASHLIGHT
};

enum  CofferMode {
	COFFER, BAG, GUN, KEY, MEDICINE, AMMO, FLASHLIGHT
};

enum  NodeMode {
	NODE, CENTER, EDGE
};

enum class PlayerMode {
	GUN1, GUN2, SRC_LIGHT, AROUND_LIGHT, DIFF_LIGHT, PLAYER
};

enum class SelectionMode {
	SELECT,
	PLACE
};

const int COFFER_OBJECT_OFFSET = 1000;
const int NO_SELECT = -1;

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();

	void init();
	void save();
	void clear();

	Tile* getTile(int x, int y);
	void resizeTiles();
	int cofferObjectIndex(CofferMode mode);
	void updateCoffers(CofferMode mode);
	void deleteLastEdge(int node1, int node2);
	void deleteSelectedNode();
	static std::string getCofferModeName(CofferMode mode);
	static CofferMode getCofferModeByName(const std::string& mode);
	static int cofferObjectIndex(CofferMode mode, int index);

	Entity* getSelectedEntity();
	Light* getSelectedLight();
	Human* getSelectedHuman();
	Zombie* getSelectedZombie();
	Coffer* getSelectedCoffer();
	Key* getSelectedKey();
	Door* getSelectedDoor();
	Gun* getSelectedGun();
	Bag* getSelectedBag();
	Ammo* getSelectedAmmo();
	Flashlight* getSelectedFlashlight();
	Medicine* getSelectedMedoc();
	GraphNode* getSelectedNode();

	bool selectLight(glm::vec2& pos);
	bool selectPlayer(glm::vec2& pos);
	bool selectHuman(glm::vec2& pos);
	bool selectZombie(glm::vec2& pos);
	bool selectCoffer(glm::vec2& pos);
	bool selectKey(glm::vec2& pos);
	bool selectGun(glm::vec2& pos);
	bool selectDoor(glm::vec2& pos);
	bool selectBag(glm::vec2& pos);
	bool selectAmmo(glm::vec2& pos);
	bool selectFlashlight(glm::vec2& pos);
	bool selectMedoc(glm::vec2& pos);
	bool selectNode(glm::vec2& pos);
	int selectCenterNode(glm::vec2& pos);

	bool tileSelected = false;
	bool playerSelected = false;
	bool onPathBuilding = false;
	bool onScreenShoot = false;
	glm::ivec4 screenRect;

	PlayerMode playerMode = PlayerMode::PLAYER;
	int playerGun1 = NO_SELECT;
	int playerGun2 = NO_SELECT;
	int playerSrc = NO_SELECT;
	int playerAround = NO_SELECT;
	int playerDiff = NO_SELECT;
	int cofferEntyties = NO_SELECT;

	std::unordered_map<int, std::unordered_map<int, CofferMode>> coffersObject;
	CofferMode cofferMode = CofferMode::COFFER;
	NodeMode nodeMode = NodeMode::NODE;

	/* Default Entities */
	Tile tile;
	Light light;			int indexLight = NO_SELECT;
	Human human;			int indexHuman = NO_SELECT;
	Zombie zombie;			int indexZombie = NO_SELECT;
	Coffer coffer;			int indexCoffer = NO_SELECT;
	Key key;				int indexKey = NO_SELECT;
	Gun gun;				int indexGun = NO_SELECT;
	Door door;				int indexDoor = NO_SELECT;
	Flashlight flashlight;	int indexFlashlight = NO_SELECT;
	Bag bag;				int indexBag = NO_SELECT;
	Ammo ammo;				int indexAmmo = NO_SELECT;
	Medicine medicine;		int indexMedoc = NO_SELECT;
	GraphNode node;			int indexNode = NO_SELECT;
	GraphEdge edge;

	ObjectMode objectMode;
	SelectionMode selectionMode;

	unsigned int width;
	unsigned int height;
	std::string name;
	std::string ssFilePath; // Screen shot

	std::vector<GraphNode> nodes;
	std::vector<GraphEdge> edges;

	Player player;
	NS2::Music music;
	NS2::ColorRGBA8 ambientColor;
	std::vector<Tile> tiles;
	std::vector<Light> lights;
	std::vector<Human> humans;
	std::vector<Zombie> zombies;
	std::vector<Coffer> coffers;
	std::vector<Key> keys;
	std::vector<Medicine> medicines;
	std::vector<Gun> guns;
	std::vector<Door> doors;
	std::vector<Bag> bags;
	std::vector<Ammo> ammos;
	std::vector<Flashlight> flashlights;
};

