#pragma once

#include "AI.h"
#include "StaggingBehavior.h"

class Zombie;

class ZombieAI : public AI
{
public:
	ZombieAI();
	virtual ~ZombieAI();

	virtual void init(Zombie* zombie);
	virtual void update(Level& level, float deltaTime) override;
	virtual void manageSensors() override;

protected:
	StaggingBehavior m_move;
	Zombie* m_zombie;
};

