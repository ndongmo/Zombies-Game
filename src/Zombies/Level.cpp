#include "Level.h"

#include <ctime>
#include <random>
#include <glm\gtx\rotate_vector.hpp>

Level::Level(NS2::Particle2DEngine& particleEngine)
{
	bullets.resize(MAX_BULLETS);
	bloodParticleBatch.init(500, 0.01f, NS2::ResourceManager::getTexture("Assets/particle.png"), []
		(NS2::Particle2D& particle, float deltaTime)
	{
		particle.destRect.x += particle.velocity.x * deltaTime;
		particle.destRect.y += particle.velocity.y * deltaTime;
		particle.velocity /= 1.2f;
		particle.color.a = (GLbyte)(particle.life * 255.0f);
	}); // Good example of using lambda expression

	textParticleBatch.init(500, 0.01f, NS2::ResourceManager::getFont("Fonts/arial.ttf", 14), []
		(NS2::Particle2D& particle, float deltaTime)
	{
		particle.destRect.x += particle.velocity.x * deltaTime;
		particle.destRect.y += particle.velocity.y * deltaTime;
		particle.color.a = (GLbyte)(particle.life * 255.0f);
	}); // Good example of using lambda expression

	particleEngine.addParticleBatch(&bloodParticleBatch);
	particleEngine.addParticleBatch(&textParticleBatch);
}

Level::~Level()
{
	std::vector<Entity*>().swap(entities);
	std::vector<Bullet>().swap(bullets);
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
	std::vector<Door>().swap(doors);
	std::vector<HumanAI>().swap(humansAis);
	std::vector<ZombieAI>().swap(zombieAis);
}
void Level::init(
	unsigned int w,
	unsigned int h,
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
	const std::string& n,
	const NS2::ColorRGBA8& a,
	const NS2::Music& m,
	NS2::EventHandler* input,
	NS2::Camera2D* camera
	)
{
	name = n;
	width = w;
	height = h;
	tiles.resize(w * h);
	nodes.resize(nbNodes);
	edges.resize(nbEdges);
	lights.resize(nbLights);
	humans.resize(nbHumans);
	zombies.reserve(nbZombies + nbHumans);
	zombies.resize(nbZombies);
	coffers.resize(nbCoffers);
	keys.resize(nbKeys);
	guns.resize(nbGuns);
	doors.resize(nbDoors);
	bags.resize(nbBags);
	ammos.resize(nbAmmos);
	flashlights.resize(nbFlashlights);
	medicines.resize(nbMedicines);
	humansAis.resize(nbHumans);
	zombieAis.reserve(nbZombies + nbHumans);
	zombieAis.resize(nbZombies);
	ambientColor = a;
	music = m;
	//music.play();
	lastFreeBullet = 0;
	grid.init(width * TILESIZE, height * TILESIZE, CELL_SIZE);
	nodeGrid.init(width * TILESIZE, height * TILESIZE, CELL_SIZE);
	playerAi.init(&player, input, camera);
	this->nbHumans = nbHumans;
	this->nbZombies = nbZombies;
	this->nbTotalZom = nbZombies;
	this->camera = camera;
	this->input = input;
}
void Level::clear()
{
	entities.clear();
	toAdd.clear();
	toRemove.clear();
	tiles.clear();
	nodes.clear();
	edges.clear();
	lights.clear();
	humans.clear();
	zombies.clear();
	coffers.clear();
	keys.clear();
	guns.clear();
	doors.clear();
	bags.clear();
	ammos.clear();
	flashlights.clear();
	medicines.clear();
	humansAis.clear();
	zombieAis.clear();
	grid.clear();
	nodeGrid.clear();
	music.stop();
	nbHumans = 0;
	nbZombies = 0;
	lastFreeBullet = 0;
}

void Level::update()
{
	for (auto& c : coffers){
		for (Entity* e : c.getEntities())
			removeFromWorld(e);
	}
}

void Level::transformToZombie(Human* a)
{
	zombies.emplace_back();
	zombieAis.emplace_back();
	Zombie& z = zombies.back();
	ZombieAI& ai = zombieAis.back();
	ai.init(&z);
	z.init(a->getShape().destRect.x, a->getShape().destRect.y, a->getShape().destRect.z,
		NS2::ResourceManager::getTexture("Assets/zombies/skeleton-idle.png"), glm::ivec2(5, 3), &ai);
	addToWorld(&z);
	nbZombies++;
}

void Level::addNode(GraphNode& node)
{
	unsigned int index = 0;
	Cell<GraphNode>* cell = nullptr;
	nodeGrid.add(&node, node.getShape().position, index, &cell);
	node.setCellIndex(index);
	node.setCell(cell);
}

void Level::addToWorld(Entity* ent)
{
	ent->setIndex(entities.size());
	entities.push_back(ent);
	unsigned int index = 0;
	Cell<Entity>* cell = nullptr;
	grid.add(ent, ent->getShape().position, index, &cell);
	ent->setCellIndex(index);
	ent->setCell(cell);
}

void Level::removeFromWorld(Entity* ent)
{
	entities[ent->getIndex()] = entities.back();
	entities[ent->getIndex()]->setIndex(ent->getIndex());
	entities.pop_back();
	grid.remove(ent->getCellIndex(), ent->getCell());
	ent->setCellIndex(-1);
	ent->setCell(nullptr);
	if (ent->getType() == EntityType::HUMAN)
		nbHumans--;
	else if (ent->getType() == EntityType::ZOMBIE)
		nbZombies--;
}

Bullet& Level::getBullet()
{
	for (unsigned int i = lastFreeBullet; i < MAX_BULLETS; i++)
	{
		if (!bullets[i].isAlive())
		{
			lastFreeBullet = i + 1;
			return bullets[i];
		}
	}

	for (unsigned int i = 0; i<lastFreeBullet; i++)
	{
		if (!bullets[i].isAlive())
		{
			lastFreeBullet = i + 1;
			return bullets[i];
		}
	}

	// No bullets are free, so override first bullet
	return bullets[0];
}

Tile* Level::getTile(int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
		return &tiles[(y * width) + x];
	else
		return nullptr;
}

int Level::getDistanceFromPlayer(const glm::vec2& position)
{
	float d = glm::distance(player.getShape().position, position);
	return (int)((d * 255) / (camera->getScreenLenght()));
}

void Level::addBlood(const glm::vec2& position, int particles)
{
	if (camera->isBoxInView(position)){
		static std::mt19937 randBloodEngine(time(nullptr));
		std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
		std::uniform_real_distribution<float> randWidth(10.0f, 30.0f);
		std::uniform_real_distribution<float> randVelo(1.5f, 5.0f);

		glm::vec2 velocity(randVelo(randBloodEngine), randVelo(randBloodEngine));
		glm::vec4 destRect(position.x, position.y, randWidth(randBloodEngine), randWidth(randBloodEngine));
		NS2::ColorRGBA8 color(138, 7, 7, 255);

		for (int i = 0; i<particles; i++)
		{
			bloodParticleBatch.addParticle(destRect, glm::rotate(velocity, randAngle(randBloodEngine)), color);
		}
	}
}

void Level::addText(const glm::vec2& position, const std::string& std)
{
	if (camera->isBoxInView(position)){
		int col = (isDark()) ? 255 : 0;

		glm::vec2 velocity(0.0f, 1.0f);
		glm::vec4 destRect(position.x, position.y, 1.0f, 1.0f);
		NS2::ColorRGBA8 color(col, col, col, 255);

		textParticleBatch.addParticle(destRect, velocity, color, std);
	}
}

bool Level::isDark()
{
	return (ambientColor.b + ambientColor.r + ambientColor.g) / 3.0f < 128;
}