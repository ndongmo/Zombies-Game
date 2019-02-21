#include "NpcAI.h"
#include "Human.h"

NpcAI::~NpcAI()
{
	m_steringAI = nullptr;
}

void NpcAI::setSteringAI(SteeringAI* ai)
{
	m_steringAI = ai;
}

void NpcAI::update(Level& level, float deltaTime)
{
	if (m_steringAI != nullptr)
	{
		m_steringAI->update(level, deltaTime);
	}
}