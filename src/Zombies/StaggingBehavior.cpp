#include "StaggingBehavior.h"
#include "Level.h"

#define CHANGE_DIR_TIME 20.0f
#define TRACKING_MAX_TIME 400.0f

void StaggingBehavior::init(float lapse, int seekingPerc, int staggingPerc)
{
	MoveBehavior::init(lapse, seekingPerc);
	m_staggingPerc = staggingPerc;
	m_staggingTime = STAGGING_NORM_TIME;
	m_changeDirTime = 0.0f;
}

void StaggingBehavior::initTracking(Agent* a)
{
	setCurrentMove(AgentMove::TRACKING);
	m_tracked = a;
	m_lastTrackedPos = a->getShape().getCenter();
	m_trackingTime = 0.0f;
}


void StaggingBehavior::updateMove(Agent* agent, Level& level, float deltaTime)
{
	m_currentTime += deltaTime;

	if (m_currentTime >= m_lapse) {
		m_moveTime += deltaTime;
		m_staggingTime -= deltaTime;

		if (updateStaggingBehavior(agent)) return;

		switch (m_currentMove)
		{
		case AgentMove::NORMAL:
			handleNormalBehavior(agent, level, deltaTime);
			agent->gotoIdleMode();
			break;
		case AgentMove::TRACKING:
			handleTrackingBehavior(agent, level, deltaTime);
			agent->gotoMoveMode();
			break;
		case AgentMove::SEEKING:
			handleSeekingBehavior(agent, level, deltaTime);
			agent->gotoIdleMode();
			break;
		case AgentMove::STAGGING:
			handleStaggingBehavior(agent, level, deltaTime);
			agent->gotoIdleMode();
			break;
		default:
			break;
		}
	}
}

bool StaggingBehavior::updateStaggingBehavior(Agent* agent)
{
	if (m_staggingTime <= 0.0f && m_currentMove != AgentMove::STAGGING){
		std::uniform_int_distribution<int> randStagger(0, 99);
		std::uniform_int_distribution<int> randNb(1, STAGGING_NB);
		std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
		std::uniform_real_distribution<float> randTime(STAGGING_MIN_TIME, STAGGING_MAX_TIME);
		if (randStagger(randEngine) <= m_staggingPerc) {
			setCurrentMove(AgentMove::STAGGING);
			m_staggingTime = randTime(randEngine);
			m_staggingNb = randNb(randEngine);
			m_staggingDir.x = randDir(randEngine);
			m_staggingDir.y = randDir(randEngine);
			m_previousDir = agent->getDirection();
			return true;
		}
		m_staggingTime = STAGGING_NORM_TIME;
	}
	return false;
}

void StaggingBehavior::handleStaggingBehavior(Agent* agent, Level& level, float deltaTime)
{
	if (m_staggingTime <= 0.0f) {
		m_staggingNb--;
		if (m_staggingNb < 1) {
			m_staggingTime = STAGGING_NORM_TIME;
			agent->setDirection(m_previousDir);
			setBackMove();
			return;
		}
		else
		{
			std::uniform_real_distribution<float> randTime(STAGGING_MIN_TIME, STAGGING_MAX_TIME);
			std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
			m_staggingTime = randTime(randEngine);
			m_staggingDir.x = randDir(randEngine);
			m_staggingDir.y = randDir(randEngine);
		}
	}

	justMoveTo(agent, m_staggingDir, deltaTime);
}

void StaggingBehavior::handleTrackingBehavior(Agent* agent, Level& level, float deltaTime)
{
	std::vector<Human*>& humans = agent->getAI()->getTouchSensor().getHumans();
	std::vector<Human*>::iterator it = std::find(humans.begin(), humans.end(), m_tracked);
	if (it == humans.end()){
		humans = agent->getAI()->getSightSensor().getHumans();
		it = std::find(humans.begin(), humans.end(), m_tracked);
	}
	if (it != humans.end()){
		m_trackingTime = 0.0f;
		m_lastTrackedPos = m_tracked->getShape().getCenter();
	}
	else if (Shape::circleContains(agent->getShape().getCenterX(), agent->getShape().getCenterY(), 
		RADIUS_POINT, m_lastTrackedPos) || m_trackingTime >= TRACKING_MAX_TIME){
		setBackMove();
		configSeeking(agent);
		m_tracked = nullptr;
		return;
	}

	m_trackingTime += deltaTime;
	moveTo(agent, m_lastTrackedPos, deltaTime);
}

void StaggingBehavior::handleNormalBehavior(Agent* agent, Level& level, float deltaTime)
{
	if (updateSeekingBehavior(agent)) return;

	glm::vec2& dir = agent->getDirection();

	if (!agent->getAI()->getTouchSensor().getTiles().empty() || 
		!agent->getAI()->getTouchSensor().getZombies().empty()) {
		if (m_changeDirTime <= 0.0f){
			std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
			dir.x = randDir(randEngine);
			dir.y = randDir(randEngine);
			m_changeDirTime = CHANGE_DIR_TIME;
		}
		else {
			m_changeDirTime -= deltaTime;
			return;
		}
	}

	glm::vec2 point = agent->getShape().getCenter() + (dir * TILESIZE);
	moveTo(agent, point, deltaTime);
}