#include "LevelGenerator.h"

#include <ctime>
#include <random>

LevelGenerator::LevelGenerator()
{

}

void LevelGenerator::init(NS2::EventHandler& input, NS2::Camera2D& camera, NS2::AudioEngine& audioEngine)
{
	m_input = &input;
	m_camera = &camera;
	m_audioEngine = &audioEngine;
}

Level* LevelGenerator::createLevel(int width, int height, int cellSize)
{
	/*Level* level = new Level(width, height, cellSize, NS2::ColorRGBA8(10, 10, 10, 255));
	
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (i == 0 || i == width-1 || j == 0 || j == height-1)
			{
				level->createTile("Assets/wall.png", i, j, false);
			}
			else
			{
				static std::mt19937 randomEngine(time(nullptr));
				std::uniform_int_distribution<int> randSize(0, 100);

				if (randSize(randomEngine) > 95)
				{
					level->createTile("Assets/wall.png", i, j, true);
				}
				else
				{
					level->createTile("Assets/ground.png", i, j, true);
				}
			}
		}
	}

	level->graph.init(*level);

	level->createPlayer(200, 200, 40, "Assets/player.png", 1, 1, 255, 255, 255, 255, 1, 0, 
		"Player", 0, 3.2f, 100, *m_input, *m_camera);

	level->createLight(200, 200, 300, 255, 0, 255, 255, LightType::POINT_LIGHT, true, true, 80);*/
	
	//level->createHuman(250, 200, 40, "Assets/human.png", 1, 1, 255, 255, 255, 255, 1, 0, "Follower", 0, 2.2f, 100);

	/*
	Medicine* med = new Medicine(50);
	med->setTexture(NS2::ResourceManager::getTexture("Assets/medicine.png"), glm::ivec2(1));
	med->setShape(200, 400, 30, 30, ShapeType::BOX);
	med->setAnimSpeed(0.1f);
	med->setColor(NS2::ColorRGBA8(255, 255, 255, 255));
	level->addToWorld(med);

	Munition* mun = new Munition(50);
	mun->setTexture(NS2::ResourceManager::getTexture("Assets/munitions.png"), glm::ivec2(1));
	mun->setShape(350, 123, 20, 20, ShapeType::BOX);
	mun->setAnimSpeed(0.1f);
	mun->setColor(NS2::ColorRGBA8(255, 255, 255, 255));
	level->addToWorld(mun);

	Coffer* coffer = new Coffer(200);
	coffer->setTexture(NS2::ResourceManager::getTexture("Assets/coffer.png"), glm::ivec2(1));
	coffer->setShape(350, 423, 60, 60, ShapeType::BOX);
	Munition* m1 = new Munition(20);
	m1->setTexture(NS2::ResourceManager::getTexture("Assets/munitions.png"), glm::ivec2(1));
	m1->setShape(350, 123, 20, 20, ShapeType::BOX);
	m1->setAnimSpeed(0.1f);
	m1->setColor(NS2::ColorRGBA8(255, 255, 255, 255));
	coffer->add(m1);
	coffer->setColor(NS2::ColorRGBA8(255, 255, 255, 255));
	level->addToWorld(coffer);

	Bag* bag = new Bag(10);
	bag->setTexture(NS2::ResourceManager::getTexture("Assets/bag.png"), glm::ivec2(1));
	bag->setShape(100, 600, 30, 30, ShapeType::BOX);
	bag->setAnimSpeed(0.1f);
	bag->setColor(NS2::ColorRGBA8(255, 255, 255, 255));
	level->addToWorld(bag);
	
	Gun* gun = new Gun();
	gun->setTexture(NS2::ResourceManager::getTexture("Assets/gun.png"), glm::ivec2(1));
	gun->setShape(230, 545, 30, 30, ShapeType::BOX);
	gun->setAnimSpeed(0.1f);
	gun->setSpeed(5.8f);
	gun->setSpread(0.05f);
	gun->setRate(5.0f);
	gun->setDamage(50.0f);
	gun->setColor(NS2::ColorRGBA8(255, 255, 255, 255));
	gun->setMaxBullet(100);
	gun->setBulletPerShot(1);
	gun->add(20);
	level->addToWorld(gun);
	*/

	return nullptr;
}
