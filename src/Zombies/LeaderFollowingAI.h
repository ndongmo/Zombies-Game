#pragma once

#include "SteeringAI.h"

class LeaderFollowingAI: public SteeringAI
{
public:
	LeaderFollowingAI(Human* human);

	virtual void init(Human* leader, float behind, int x, int y);
	virtual void update(Level& level, float deltaTime) override;

protected:
	Human* m_leader;
	float m_behind;
	int m_x, m_y;
};

