#include "ZombieAI.h"
#include "Level.h"

ZombieAI::ZombieAI() : AI()
{
	m_name = "ZOMBIE_AI";
}
ZombieAI::~ZombieAI()
{
	m_zombie = nullptr;
}

void ZombieAI::init(Zombie* zombie)
{
	m_zombie = zombie;
	m_sight.init(200.0f, 45.0f, 1.5f);
	m_move.init(200.0f);
}

void ZombieAI::update(Level& level, float deltaTime)
{
	manageSensors();

	m_move.updateMove(m_zombie, level, deltaTime);
	m_sight.update(m_zombie, level, deltaTime);
	m_touch.update(m_zombie, level, deltaTime);
}

void ZombieAI::manageSensors()
{
	Agent* h = nullptr;
	if (m_touch.getHumans().size() > 0) {
		h = m_touch.getHumans()[0];
		m_zombie->gotoAttackMode();
	}
	else if (m_sight.getHumans().size() > 0) {
		h = m_sight.getHumans()[0];
	}
	if (h != nullptr && m_move.getTracked() != h) {
		m_move.initTracking(h);
	}
	if (m_move.getMoveType() != AgentMove::TRACKING && m_touch.getBullets().size() > 0){
		m_zombie->setDirection(- m_touch.getBullets()[0]->getDirection());
		m_move.configSeeking(m_zombie);
	}
}
