#include "MoveBehavior.h"
#include "Level.h"

std::mt19937 MoveBehavior::randEngine(time(nullptr));

void MoveBehavior::init(float lapse, int seekingPerc)
{
	m_lapse = lapse;
	m_currentTime = 0.0f;
	m_seekingPerc = seekingPerc;
	m_moveTime = 0.0f;
	m_angleDir = 1.0f;
	m_currentMove = AgentMove::NORMAL;
	m_previousMove = AgentMove::NORMAL;
}

void MoveBehavior::setBackMove()
{
	if (m_currentMove != m_previousMove){
		m_currentMove = m_previousMove;
		m_previousMove = AgentMove::NORMAL;;
	}
	else
		m_currentMove = AgentMove::NORMAL;
}

void MoveBehavior::setCurrentMove(AgentMove move)
{
	m_previousMove = m_currentMove;
	m_currentMove = move;
}

void  MoveBehavior::configSeeking(Agent* agent)
{
	std::uniform_int_distribution<int> randDir(0, 1);
	std::uniform_real_distribution<float> randTime(SEEK_MIN_TIME, SEEK_MAX_TIME);

	setCurrentMove(AgentMove::SEEKING);
	m_seekTime = randTime(randEngine);
	m_currentSeekTime = m_seekTime;
	m_angleDir = (randDir(randEngine) != 0) ? -1 : 1;
	m_previousDir = agent->getDirection();
}

bool MoveBehavior::updateSeekingBehavior(Agent* agent)
{
	if (m_moveTime > SEEK_NORM_TIME){
		std::uniform_int_distribution<int> randSeek(0, 99);

		if (randSeek(randEngine) <= m_seekingPerc) {
			configSeeking(agent);
			return true;
		}
		m_moveTime = 0.0f;
	}
	return false;
}

void  MoveBehavior::handleSeekingBehavior(Agent* agent, Level& level, float deltaTime)
{
	m_currentSeekTime -= deltaTime;

	if (m_currentSeekTime <= 0.0f) {
		setBackMove();
		return;
	}

	glm::vec2 dir = agent->getDirection();
	float diff = acos(glm::dot(m_previousDir, dir));
	float angle = (deltaTime * 4 * SEEK_ANGLE_VIEW) / m_seekTime;
	if (abs(diff) >= SEEK_ANGLE_VIEW){
		m_angleDir *= -1;
		angle += abs(diff) - SEEK_ANGLE_VIEW;
	}
	
	dir = glm::rotate(dir, m_angleDir * angle);
	dir = glm::normalize(dir);
	agent->setDirection(dir);
}

void MoveBehavior::moveTo(Agent* agent, glm::vec2& point, float deltaTime)
{
	glm::vec2 dir = glm::normalize(point - agent->getShape().getCenter());
	glm::vec2 vel = dir * agent->getSpeed() * deltaTime;
	agent->setDirection(dir);
	agent->getShape().incrPosition(vel);
}

void MoveBehavior::justMoveTo(Agent* agent, glm::vec2& dir, float deltaTime)
{
	glm::vec2 vel = dir * agent->getSpeed() * deltaTime;
	agent->getShape().incrPosition(vel);
}