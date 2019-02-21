#pragma once

#include "AI.h"
#include "MigrantBehavior.h"

class Human;

class HumanAI : public AI
{
public:
	HumanAI();
	virtual ~HumanAI();

	virtual void init(Human* human);
	virtual void update(Level& level, float deltaTime) override;
	virtual void manageSensors() override;
	MigrantBehavior& getMoveBehavior(){ return m_move; }


protected:
	MigrantBehavior m_move;
	Human* m_human;
};

