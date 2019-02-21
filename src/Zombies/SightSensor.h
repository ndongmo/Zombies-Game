#pragma once

#include "Sensor.h"

#include <DebugRenderer.h>

class Entity;
template<class T>
struct Cell;

class SightSensor : public Sensor
{
public:
	SightSensor(AI* ai);
	/*!
	* \brief Initialize Sight sensor AI.
	* \param agent : owner for the sight
	* \param radius : how far agent can see
	* \param angle : sight's angle in degree
	* \param lapse : lapse to refresh the sight
	*/
	void init(float radius, float angle, float lapse);

	virtual void draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8& color) override;

	virtual void notifyForTile() override;
	virtual void notifyForZombie() override;
	virtual void notifyForHuman() override;
	virtual void notifyForObject() override;
	virtual void notifyForBullet() override;

	/* GETTERS */
	float getRadius(){ return m_radius; }
	glm::vec2& getCenter(){ return m_center; }

private:
	void find(Agent* agent, Level& level, Cell<Entity>& cell);

protected:
	virtual void updateSensor(Agent* agent, Level& level, float deltaTime) override;

	float m_radius;
	float m_angle;

	glm::vec2 m_center;
	glm::vec2 m_left;
	glm::vec2 m_right;
};

