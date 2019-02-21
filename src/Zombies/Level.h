#pragma once

#include "GraphEdge.h"
#include "GraphNode.h"
#include "Grid.h"
#include "Tile.h"
#include "Light.h"

#include "Entity.h"

#include "Shape.h"

#include "Bullet.h"

#include "Object.h"
#include "Door.h"
#include "Medicine.h"
#include "Gun.h"
#include "Coffer.h"
#include "Key.h"
#include "Flashlight.h"

#include "Zombie.h"
#include "Human.h"
#include "Player.h"

#include "PlayerAI.h"
#include "HumanAI.h"
#include "ZombieAI.h"
#include "LeaderFollowingAI.h"

#include <ResourceManager.h>
#include <ParticleBatch2D.h>
#include <FontParticleBatch2D.h>
#include <Particle2DEngine.h>

#define MAX_BULLETS 50
#define CELL_SIZE 200

class Level
{
	friend class LevelHandler;
public:
	Level(NS2::Particle2DEngine& particleEngine);
	~Level();

	void init(
		unsigned int width, 
		unsigned int height, 
		unsigned int nbNodes,
		unsigned int nbEdges,
		unsigned int nbLights,
		unsigned int nbHumans,
		unsigned int nbZombies,
		unsigned int nbCoffers,
		unsigned int nbKeys,
		unsigned int nbGuns,
		unsigned int nbDoors,
		unsigned int nbMedicines,
		unsigned int nbBags,
		unsigned int nbAmmos,
		unsigned int nbFlashlights,
		const std::string& name,
		const NS2::ColorRGBA8& ambient,
		const NS2::Music& music,
		NS2::EventHandler* input,
		NS2::Camera2D* camera
	);
	void clear();

	void update();
	void transformToZombie(Human* a);
	void addToWorld(Entity* ent);
	void addNode(GraphNode& node);
	void removeFromWorld(Entity* ent);
	void addBlood(const glm::vec2& position, int particles);
	void addText(const glm::vec2& position, const std::string& std);
	bool isDark();

	/* GETTERS */
	int getDistanceFromPlayer(const glm::vec2& position);
	Player& getPlayer(){ return player; }
	Tile* getTile(int x, int y);
	unsigned int getTileSize(){ return width * height; }
	Bullet& getBullet();
	
	int nbHumans, nbZombies, nbTotalZom;
	std::vector<Entity*> entities;
	Grid<Entity> grid;
	Grid<GraphNode> nodeGrid;
	std::vector<GraphNode> nodes;
	std::vector<GraphEdge> edges;

	Player player;
	std::vector<Bullet> bullets;
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

	std::vector<HumanAI> humansAis;
	std::vector<ZombieAI> zombieAis;
	PlayerAI playerAi;

	std::vector<Entity*> toAdd;
	std::vector<Entity*> toRemove;

	unsigned int width;
	unsigned int height;
	unsigned int lastFreeBullet;
	std::string name;
	NS2::Music music;
	NS2::ColorRGBA8 ambientColor;
	NS2::Camera2D* camera;
	NS2::EventHandler* input;
	NS2::ParticleBatch2D bloodParticleBatch;
	NS2::FontParticleBatch2D textParticleBatch;
};

