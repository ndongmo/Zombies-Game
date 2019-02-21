#include "LevelEditor.h"


LevelEditor::LevelEditor()
{
	
}

LevelEditor::~LevelEditor()
{
	std::vector<GraphNode>().swap(nodes);
	std::vector<GraphEdge>().swap(edges);
	std::vector<Tile>().swap(tiles);
	std::vector<Light>().swap(lights);
	std::vector<Human>().swap(humans);
	std::vector<Zombie>().swap(zombies);
	std::vector<Coffer>().swap(coffers);
	std::vector<Key>().swap(keys);
	std::vector<Medicine>().swap(medicines);
	std::vector<Gun>().swap(guns);
	std::vector<Bag>().swap(bags);
	std::vector<Ammo>().swap(ammos);
	std::vector<Flashlight>().swap(flashlights);
	std::vector<Door>().swap(doors);
}

void LevelEditor::init()
{
	NS2::GLTexture& tex = NS2::ResourceManager::getTexture("Assets/ground.png");
	NS2::Sound& sound = NS2::ResourceManager::getSound("Sounds/cg1.wav");
	NS2::ColorRGBA8 color(255, 255, 255, 255);
	glm::vec2 direction(1, 0);
	glm::ivec2 texDim(1, 1);

	music = NS2::ResourceManager::getMusic("Musics/XYZ.ogg");
	tile.init(tex, 0, 0, false);
	tex = NS2::ResourceManager::getTexture("Assets/player/fist/idle_fist.png");
	texDim.x = 5; texDim.y = 2;
	player.init(0.0f, 0.0f, 40.0f, tex, texDim, nullptr);
	tex = NS2::ResourceManager::getTexture("Assets/zombies/skeleton-idle.png");
	texDim.x = 5; texDim.y = 3;
	zombie.init(0.0f, 0.0f, 40.0f, tex, texDim, nullptr);
	tex = NS2::ResourceManager::getTexture("Assets/humans/idle_human.png");
	texDim.x = 1; texDim.y = 1;
	human.init(0.0f, 0.0f, 40.0f, tex, texDim, nullptr);
	light.init(0.0f, 0.0f, 40.0f, 0, color);
	tex = NS2::ResourceManager::getTexture("Assets/coffer.png");
	coffer.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound, sound);
	tex = NS2::ResourceManager::getTexture("Assets/key.png");
	key.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound);
	tex = NS2::ResourceManager::getTexture("Assets/bag.png");
	bag.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound, direction);
	tex = NS2::ResourceManager::getTexture("Assets/ammo_1.png");
	ammo.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound, direction);
	tex = NS2::ResourceManager::getTexture("Assets/flashlight.png");
	flashlight.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound, direction);
	tex = NS2::ResourceManager::getTexture("Assets/pistol.png");
	gun.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound);
	tex = NS2::ResourceManager::getTexture("Assets/lock.png");
	texDim.x = 2;
	door.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound, direction);
	door.setOpened(true);
	tex = NS2::ResourceManager::getTexture("Assets/medicine.png");
	medicine.init(0.0f, 0.0f, 40.0f, 40.0f, tex, texDim, sound);
	node.init(0, 0.0f, 0.0f, 40.0f, 40.0f);

	width = 10;
	height = 10;
	objectMode = ObjectMode::LEVEL;
	selectionMode = SelectionMode::PLACE;
	ambientColor = NS2::ColorRGBA8(255, 255, 255, 255);
	resizeTiles();
}

void LevelEditor::clear()
{
	tileSelected = false;
	playerSelected = false;
	onPathBuilding = false;
	onScreenShoot = false;

	playerGun1 = NO_SELECT;
	playerGun2 = NO_SELECT;
	playerSrc = NO_SELECT;
	playerAround = NO_SELECT;
	playerDiff = NO_SELECT;
	cofferEntyties = NO_SELECT;
	indexLight = NO_SELECT;
	indexHuman = NO_SELECT;
	indexZombie = NO_SELECT;
	indexCoffer = NO_SELECT;
	indexKey = NO_SELECT;
	indexGun = NO_SELECT;
	indexDoor = NO_SELECT;
	indexBag = NO_SELECT;
	indexAmmo = NO_SELECT;
	indexFlashlight = NO_SELECT;
	indexMedoc = NO_SELECT;
	indexNode = NO_SELECT;

	edges.clear();
	nodes.clear();
	tiles.clear();
	lights.clear();
	humans.clear();
	zombies.clear();
	coffers.clear();
	keys.clear();
	bags.clear();
	ammos.clear();
	flashlights.clear();
	medicines.clear();
	guns.clear();
	doors.clear();
	
	width = 10;
	height = 10;
	objectMode = ObjectMode::LEVEL;
	selectionMode = SelectionMode::PLACE;
	ambientColor = NS2::ColorRGBA8(255, 255, 255, 255);
	resizeTiles();
}

Tile* LevelEditor::getTile(int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height && ((y * width) + x) < tiles.size())
		return &tiles[(y * width) + x];
	else
		return nullptr;
}

std::string LevelEditor::getCofferModeName(CofferMode mode)
{
	if (mode == CofferMode::GUN)
		return "GUN";
	else if (mode == CofferMode::KEY)
		return "KEY";
	else if (mode == CofferMode::BAG)
		return "BAG";
	else if (mode == CofferMode::AMMO)
		return "AMMO";
	else if (mode == CofferMode::FLASHLIGHT)
		return "FLASHLIGHT";
	else if (mode == CofferMode::MEDICINE)
		return "MEDICINE";
	else
		return "COFFER";
}

CofferMode LevelEditor::getCofferModeByName(const std::string& mode)
{
	if (mode == "GUN")
		return CofferMode::GUN;
	else if (mode == "KEY")
		return CofferMode::KEY;
	else if (mode == "BAG")
		return CofferMode::BAG;
	else if (mode == "AMMO")
		return CofferMode::AMMO;
	else if (mode == "FLASHLIGHT")
		return CofferMode::FLASHLIGHT;
	else if (mode == "MEDICINE")
		return CofferMode::MEDICINE;
	else
		return CofferMode::COFFER;
}

void LevelEditor::resizeTiles()
{
	tiles.resize(width * height);
	for (size_t i = 0; i < tiles.size(); i++)
	{
		int x = i % width;
		int y = (int)(i / width);
		tiles[i].setPosition(x, y);
	}
}

Light* LevelEditor::getSelectedLight()
{
	Light* l = nullptr;

	if (selectionMode == SelectionMode::PLACE)
		l = &light;
	else if (indexLight != NO_SELECT)
		l = &lights[indexLight];
	return l;
}
Human* LevelEditor::getSelectedHuman()
{
	Human* h = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		h = &human;
	else if (indexHuman != NO_SELECT)
		h = &humans[indexHuman];
	return h;
}
Zombie* LevelEditor::getSelectedZombie()
{
	Zombie* z = &zombie;
	if (selectionMode == SelectionMode::PLACE)
		z = &zombie;
	else if (indexZombie != NO_SELECT)
		z = &zombies[indexZombie];
	return z;
}
Coffer* LevelEditor::getSelectedCoffer()
{
	Coffer* c = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		c = &coffer;
	else if (indexCoffer != NO_SELECT)
		c = &coffers[indexCoffer];
	return c;
}
Key* LevelEditor::getSelectedKey()
{
	Key* k = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		k = &key;
	else if (indexKey != NO_SELECT)
		k = &keys[indexKey];
	return k;
}
Door* LevelEditor::getSelectedDoor()
{
	Door* d = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		d = &door;
	else if (indexDoor != NO_SELECT)
		d = &doors[indexDoor];
	return d;
}
Gun* LevelEditor::getSelectedGun()
{
	Gun* g = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		g = &gun;
	else if (indexGun != NO_SELECT)
		g = &guns[indexGun];
	return g;
}
Bag* LevelEditor::getSelectedBag()
{
	Bag* g = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		g = &bag;
	else if (indexBag != NO_SELECT)
		g = &bags[indexBag];
	return g;
}
Ammo* LevelEditor::getSelectedAmmo()
{
	Ammo* g = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		g = &ammo;
	else if (indexAmmo != NO_SELECT)
		g = &ammos[indexAmmo];
	return g;
}
Flashlight* LevelEditor::getSelectedFlashlight()
{
	Flashlight* g = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		g = &flashlight;
	else if (indexFlashlight != NO_SELECT)
		g = &flashlights[indexFlashlight];
	return g;
}
Medicine* LevelEditor::getSelectedMedoc()
{
	Medicine* m = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		m = &medicine;
	else if (indexMedoc != NO_SELECT)
		m = &medicines[indexMedoc];
	return m;
}
GraphNode* LevelEditor::getSelectedNode()
{
	GraphNode* n = nullptr;
	if (selectionMode == SelectionMode::PLACE)
		n = &node;
	else if (indexNode != NO_SELECT)
		n = &nodes[indexNode];
	return n;
}

int LevelEditor::cofferObjectIndex(CofferMode mode)
{
	if (mode == CofferMode::AMMO)
		return indexAmmo + (CofferMode::AMMO * COFFER_OBJECT_OFFSET);
	else if(mode == CofferMode::BAG)
		return indexBag + (CofferMode::BAG * COFFER_OBJECT_OFFSET);
	else if(mode == CofferMode::FLASHLIGHT)
		return indexFlashlight + (CofferMode::FLASHLIGHT * COFFER_OBJECT_OFFSET);
	else if (mode == CofferMode::GUN)
		return indexGun + (CofferMode::GUN * COFFER_OBJECT_OFFSET);
	else if (mode == CofferMode::KEY)
		return indexKey + (CofferMode::KEY * COFFER_OBJECT_OFFSET);
	else if (mode == CofferMode::MEDICINE)
		return indexMedoc + (CofferMode::MEDICINE * COFFER_OBJECT_OFFSET);
	else
		return NO_SELECT;
}

int LevelEditor::cofferObjectIndex(CofferMode mode, int index)
{
	if (mode == CofferMode::BAG)
		index = index - (CofferMode::BAG * COFFER_OBJECT_OFFSET);
	else if(mode == CofferMode::AMMO)
		index = index - (CofferMode::AMMO * COFFER_OBJECT_OFFSET);
	else if(mode == CofferMode::FLASHLIGHT)
		index = index - (CofferMode::FLASHLIGHT * COFFER_OBJECT_OFFSET);
	else if (mode == CofferMode::GUN)
		index = index - (CofferMode::GUN * COFFER_OBJECT_OFFSET);
	else if (mode == CofferMode::KEY)
		index = index - (CofferMode::KEY * COFFER_OBJECT_OFFSET);
	else if (mode == CofferMode::MEDICINE)
		index = index - (CofferMode::MEDICINE * COFFER_OBJECT_OFFSET);
	else
		index = NO_SELECT;
	
	return index;
}

void LevelEditor::updateCoffers(CofferMode mode)
{
	int indexToErase = NO_SELECT;
	int indexToReplace = NO_SELECT;

	if (mode == CofferMode::BAG) {
		indexToErase = indexBag + (CofferMode::BAG * COFFER_OBJECT_OFFSET);
		indexToReplace = bags.size() - 1 + (CofferMode::BAG * COFFER_OBJECT_OFFSET);
	}
	else if(mode == CofferMode::AMMO) {
		indexToErase = indexAmmo + (CofferMode::AMMO * COFFER_OBJECT_OFFSET);
		indexToReplace = ammos.size() - 1 + (CofferMode::AMMO * COFFER_OBJECT_OFFSET);
	}
	else if(mode == CofferMode::FLASHLIGHT) {
		indexToErase = indexFlashlight + (CofferMode::FLASHLIGHT * COFFER_OBJECT_OFFSET);
		indexToReplace = flashlights.size() - 1 + (CofferMode::FLASHLIGHT * COFFER_OBJECT_OFFSET);
	}
	else if (mode == CofferMode::GUN) {
		indexToErase = indexGun + (CofferMode::GUN * COFFER_OBJECT_OFFSET);
		indexToReplace = guns.size() - 1 + (CofferMode::GUN * COFFER_OBJECT_OFFSET);
	}
	else if (mode == CofferMode::KEY) {
		indexToErase = indexKey + (CofferMode::KEY * COFFER_OBJECT_OFFSET);
		indexToReplace = keys.size() - 1 + (CofferMode::KEY * COFFER_OBJECT_OFFSET);
	}
	else if (mode == CofferMode::MEDICINE) {
		indexToErase = indexMedoc + (CofferMode::MEDICINE * COFFER_OBJECT_OFFSET);
		indexToReplace = medicines.size() - 1 + (CofferMode::MEDICINE * COFFER_OBJECT_OFFSET);
	}
	else
	{
		return;
	}

	for (auto& m : coffersObject)
	{
		if (m.second.find(indexToErase) != m.second.end()) {
			m.second.erase(indexToErase);
		}

		if (m.second.find(indexToReplace) != m.second.end()) {
			m.second[indexToErase] = m.second[indexToReplace];
			m.second.erase(indexToReplace);
		}
	}
}

Entity* LevelEditor::getSelectedEntity()
{
	Entity* e = nullptr;
	if (objectMode == ObjectMode::PLAYER)
	{
		e = &player;
	}
	else if (objectMode == ObjectMode::HUMAN)
	{
		e = getSelectedHuman();
	}
	else if (objectMode == ObjectMode::ZOMBIE)
	{
		e = getSelectedZombie();
	}
	else if (objectMode == ObjectMode::COFFER)
	{
		e = getSelectedCoffer();
	}
	else if (objectMode == ObjectMode::KEY)
	{
		e = getSelectedKey();
	}
	else if (objectMode == ObjectMode::DOOR)
	{
		e = getSelectedDoor();
	}
	else if (objectMode == ObjectMode::GUN)
	{
		e = getSelectedGun();
	}
	else if (objectMode == ObjectMode::MEDICINE)
	{
		e = getSelectedMedoc();
	}
	else if (objectMode == ObjectMode::BAG)
	{
		e = getSelectedBag();
	}
	else if (objectMode == ObjectMode::AMMO)
	{
		e = getSelectedAmmo();
	}
	else if (objectMode == ObjectMode::FLASHLIGHT)
	{
		e = getSelectedFlashlight();
	}

	return e;
}

bool LevelEditor::selectLight(glm::vec2& pos)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (pos.x > lights[i].destRect.x && pos.x < lights[i].destRect.x + lights[i].destRect.z &&
			pos.y > lights[i].destRect.y && pos.y < lights[i].destRect.y + lights[i].destRect.z)
		{
			indexLight = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectPlayer(glm::vec2& pos)
{
	return player.getShape().contains(pos);
}

bool LevelEditor::selectHuman(glm::vec2& pos)
{
	for (size_t i = 0; i < humans.size(); i++)
	{
		if (humans[i].getShape().contains(pos))
		{
			indexHuman = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectZombie(glm::vec2& pos)
{
	for (size_t i = 0; i < zombies.size(); i++)
	{
		if (zombies[i].getShape().contains(pos))
		{
			indexZombie = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectCoffer(glm::vec2& pos)
{
	for (size_t i = 0; i < coffers.size(); i++)
	{
		if (coffers[i].getShape().contains(pos))
		{
			indexCoffer = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectKey(glm::vec2& pos)
{
	for (size_t i = 0; i < keys.size(); i++)
	{
		if (keys[i].getShape().contains(pos))
		{
			indexKey = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectGun(glm::vec2& pos)
{
	for (size_t i = 0; i < guns.size(); i++)
	{
		if (guns[i].getShape().contains(pos))
		{
			indexGun = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectDoor(glm::vec2& pos)
{
	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i].getShape().contains(pos))
		{
			indexDoor = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectBag(glm::vec2& pos)
{
	for (size_t i = 0; i < bags.size(); i++)
	{
		if (bags[i].getShape().contains(pos))
		{
			indexBag = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectAmmo(glm::vec2& pos)
{
	for (size_t i = 0; i < ammos.size(); i++)
	{
		if (ammos[i].getShape().contains(pos))
		{
			indexAmmo = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectFlashlight(glm::vec2& pos)
{
	for (size_t i = 0; i < flashlights.size(); i++)
	{
		if (flashlights[i].getShape().contains(pos))
		{
			indexFlashlight = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectMedoc(glm::vec2& pos)
{
	for (size_t i = 0; i < medicines.size(); i++)
	{
		if (medicines[i].getShape().contains(pos))
		{
			indexMedoc = i;
			return true;
		}
	}
	return false;
}

bool LevelEditor::selectNode(glm::vec2& pos)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].getShape().contains(pos))
		{
			indexNode = i;
			return true;
		}
	}
	return false;
}

int LevelEditor::selectCenterNode(glm::vec2& pos)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (i != indexNode){
			glm::vec2 c(nodes[i].getCenter().x - CENTER_WIDTH / 2.0f,
				nodes[i].getCenter().y - CENTER_WIDTH / 2.0f);

			if (pos.x > c.x && pos.x < c.x + CENTER_WIDTH &&
				pos.y > c.y && pos.y < c.y + CENTER_WIDTH)
			{
				return i;
			}
		}
	}
	return NO_SELECT;
}

void LevelEditor::deleteSelectedNode()
{
	auto neighbors = nodes[indexNode].getNeighbors();

	for (auto& ne : neighbors) { // run through neighbors
		for (auto& e : ne.second) { // run through edges
			deleteLastEdge(indexNode, ne.first);
		}
	}

	int lastNodeIndex = nodes.size() - 1;
	nodes[indexNode] = nodes.back();
	nodes.pop_back();

	// update nodes which has got replaced nodes
	for (auto& no : nodes) {
		if (no.getNeighbors().find(lastNodeIndex) != no.getNeighbors().end()){
			no.getNeighbors()[indexNode] = no.getNeighbors()[lastNodeIndex];
			no.getNeighbors().erase(lastNodeIndex);
		}
	}

	indexNode = NO_SELECT;
}
void LevelEditor::deleteLastEdge(int node1, int node2)
{
	if (nodes[node1].getNeighbors()[node2].size() > 0)
	{
		int edgeIndex = nodes[node1].getNeighbors()[node2].back();
		int lastEdgeIndex = edges.size() - 1;
		nodes[node1].getNeighbors()[node2].pop_back();
		nodes[node2].getNeighbors()[node1].pop_back();

		if (nodes[node1].getNeighbors()[node2].empty())
			nodes[node1].getNeighbors().erase(node2);
		if (nodes[node2].getNeighbors()[node1].empty())
			nodes[node2].getNeighbors().erase(node1);

		edges[edgeIndex] = edges.back();
		edges.pop_back();

		for (auto& no : nodes) { // // run through nodes
			for (auto& ne : no.getNeighbors()) { // run through neighbors
				for (auto& e : ne.second) { // run through edges
					if (e == lastEdgeIndex)
						e = edgeIndex; // set new index edge
				}
			}
		}
	}
}