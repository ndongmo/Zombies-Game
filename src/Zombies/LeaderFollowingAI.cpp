#include "LeaderFollowingAI.h"
#include "Human.h"

LeaderFollowingAI::LeaderFollowingAI(Human* human) : SteeringAI(human)
{
	m_name = "LEADER_FOLLOWING_AI";
}

void LeaderFollowingAI::init(Human* leader, float behind, int x, int y)
{
	m_leader = leader;
	m_behind = behind;
	m_move = true;
	m_x = x;
	m_y = y;
}

void LeaderFollowingAI::update(Level& level, float deltaTime)
{
	//collide(level, deltaTime);

	if (m_leader != nullptr && m_leader->isAlive())
	{
		glm::vec2 pos(m_leader->getShape().getCenterX(), m_leader->getShape().getCenterY());
		float disX = abs(m_human->getShape().getCenterX() - m_leader->getShape().getCenterX());
		float disY = abs(m_human->getShape().getCenterY() - m_leader->getShape().getCenterY());

		if (disX >= m_behind || disY >= m_behind)
		{
			m_human->setDirection(glm::normalize(pos - m_human->getShape().getCenter()));
			m_human->getShape().incrPosition(m_human->getDirection() * m_human->getSpeed() * deltaTime);
		}
		else if (disX <= m_behind - 10 && disY <= m_behind - 10)
		{ // evade from leader
			glm::vec2 desiredVelocity = glm::normalize(m_human->getShape().getCenter() - pos);
			m_human->getShape().incrPosition((desiredVelocity * m_human->getSpeed() * deltaTime));
		}
	}
}