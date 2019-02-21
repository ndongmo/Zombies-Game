#pragma once

#include <DebugRenderer.h>

class Tile;
class Agent;
class Human;
class Zombie;
class Object;
class Level;
class Bullet;
class AI;

class Sensor
{
public:
	Sensor(AI* ai);
	void update(Agent* agent, Level& level, float deltaTime);

	virtual void draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8& color);
	virtual void notifyForTile() = 0;
	virtual void notifyForZombie() = 0;
	virtual void notifyForHuman() = 0;
	virtual void notifyForObject() = 0;
	virtual void notifyForBullet() = 0;

	/* GETTERS */
	std::vector<Tile*>& getTiles(){ return m_tiles; }
	std::vector<Human*>& getHumans(){ return m_humans; }
	std::vector<Zombie*>& getZombies(){ return m_zombies; }
	std::vector<Object*>& getObjects(){ return m_objects; }
	std::vector<Bullet*>& getBullets(){ return m_bullets; }

protected:
	virtual void updateSensor(Agent* agent, Level& level, float deltaTime) = 0;

	float m_lapse;
	float m_currentTime;

	std::vector<Tile*> m_tiles;
	std::vector<Zombie*> m_zombies;
	std::vector<Human*> m_humans;
	std::vector<Object*> m_objects;
	std::vector<Bullet*> m_bullets;
	AI* m_ai;
};

