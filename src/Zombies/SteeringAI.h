#pragma once

#include "AI.h"
#include "Shape.h"
#include <glm\glm.hpp>

class Human;

class SteeringAI : public AI
{
public:
	SteeringAI(Human* human);
	virtual ~SteeringAI();
	virtual bool collide(Level& level, float deltaTime);
	virtual bool onMove(){ return m_move; };

protected:
	Human* m_human;
	Shape m_collisionCircle;
	bool m_move;
};

