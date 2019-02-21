#include "HumanAI.h"
#include "MigrantBehavior.h"
#include "Level.h"

HumanAI::HumanAI() : AI()
{
	m_name = "HUMAN_AI";
}
HumanAI::~HumanAI()
{
	m_human = nullptr;
}

void HumanAI::init(Human* human)
{
	m_human = human;
	m_sight.init(200.0f, 45.0f, 0.5f);
	m_move.init(200.0f);
}

void HumanAI::update(Level& level, float deltaTime)
{
	manageSensors();

	m_move.updateMove(m_human, level, deltaTime);
	m_sight.update(m_human, level, deltaTime);
	m_touch.update(m_human, level, deltaTime);
}

void HumanAI::manageSensors()
{
	if (m_move.getMoveType() == AgentMove::LOST)
		return;

	if (!m_move.isSleeping()) {
		if (m_move.getMoveType() != AgentMove::EVADE &&
			m_move.getMoveType() != AgentMove::SAFEPLACE &&
			!m_touch.getHumans().empty()) {
			Human* h = m_touch.getHumans()[0];
			HumanAI* ai = static_cast<HumanAI*>(h->getAI());
			bool flag = (h != m_move.getEvadeFrom() && !ai->getMoveBehavior().isSleeping() &&
				ai->getMoveBehavior().getEvadeFrom() != m_human);
			if (flag) {
				m_move.setCurrentMove(AgentMove::EVADE);
				m_move.setEvadeFrom(h);
			}
		}
	}

	Agent* z = nullptr;
	if (m_touch.getZombies().size() > 0) {
		z = m_touch.getZombies()[0];
	}
	else if (m_sight.getZombies().size() > 0) {
		z = m_sight.getZombies()[0];
	}
	if (z != nullptr) {
		if (m_move.getMoveType() == AgentMove::FLEE) {
			Agent* fleeFrom = m_move.getFleeFrom();
			if (fleeFrom == z || 
				glm::distance(z->getShape().getCenter(), m_human->getShape().getCenter()) >
				glm::distance(fleeFrom->getShape().getCenter(), m_human->getShape().getCenter()))
				return;
		}

		glm::vec2 pt;
		bool flag = false;

		if ((flag = m_move.getPreviousPoint(pt))) {
			float dist = glm::distance(z->getShape().getCenter(), pt);
			flag = dist > glm::distance(z->getShape().getCenter(), m_move.getCurrentPoint());
		}
		if (flag) m_move.turnBack();
		m_move.setFleeFrom(z);
		m_move.setCurrentMove(AgentMove::FLEE);
	}
}
