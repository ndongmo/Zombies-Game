#pragma once

#include "HumanAI.h"
#include "SteeringAI.h"

class NpcAI : public HumanAI
{
public:
	~NpcAI();
	virtual void update(Level& level, float deltaTime) override;
	void setSteringAI(SteeringAI* ai);

protected:
	unsigned int m_currentNode;
	SteeringAI* m_steringAI;
};

