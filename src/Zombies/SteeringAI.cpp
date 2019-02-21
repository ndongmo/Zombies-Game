#include "SteeringAI.h"
#include "Level.h"

#define MAX_SEE_AHEAD 30.0f
#define MAX_AVOID_FORCE 1.0f

SteeringAI::SteeringAI(Human* human) : m_human(human),
m_collisionCircle(human->getShape().position.x, human->getShape().position.y,
human->getShape().destRect.z, human->getShape().destRect.w, ShapeType::CIRCLE)
{
	m_move = false;
	m_name = "STEERING_AI";
}

SteeringAI::~SteeringAI()
{
	m_human = nullptr;
}

bool SteeringAI::collide(Level& level, float deltaTime)
{
	// Collision avoidance
	// issue : given that humans are bigger than tile, i cannot manage the variety of collisions now 
	/*
	glm::vec2 ahead = m_human->getShape().getCenter() + m_human->getDirection() * MAX_SEE_AHEAD;
	Tile* t = level.getTile(ahead.x / TILESIZE, ahead.y / TILESIZE);
	if (t != nullptr && !t->isCrossable())
	{
		glm::vec2 center;
		t->setCenter(center);
		m_collisionCircle.setPosition(ahead);
		if (m_collisionCircle.testCollideTile(center, TILESIZE))
		{
			m_collisionCircle.incrPosition(center);
			m_human->setDirection(glm::normalize(ahead));
			m_human->getShape().incrPosition(m_human->getDirection() * m_human->getSpeed() * deltaTime * MAX_AVOID_FORCE);
			return true;
		}
	}
	*/
	return false;
}