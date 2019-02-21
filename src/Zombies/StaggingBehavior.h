#pragma once

#include "MoveBehavior.h"

#define STAGGING_MIN_TIME 20.0f
#define STAGGING_MAX_TIME 40.0f
#define STAGGING_NORM_TIME 150.0f
#define STAGGING_NB 3

class StaggingBehavior: public MoveBehavior
{
public:
	void init(float lapse, int seekingPerc = 10, int staggingPerc = 50);
	void initTracking(Agent* a);
	virtual void updateMove(Agent* agent, Level& level, float deltaTime) override;

	/* SETTER */
	void setTracked(Agent* a){ m_tracked = a; }

	/* GETTER */
	Agent* getTracked(){ return m_tracked; }

protected:
	virtual bool updateStaggingBehavior(Agent* agent);
	virtual void handleStaggingBehavior(Agent* agent, Level& level, float deltaTime);
	virtual void handleTrackingBehavior(Agent* agent, Level& level, float deltaTime);
	virtual void handleNormalBehavior(Agent* agent, Level& level, float deltaTime) override;

	int m_staggingPerc;
	int m_staggingNb;
	float m_staggingTime;
	float m_changeDirTime;
	float m_trackingTime;

	glm::vec2 m_staggingDir;
	glm::vec2 m_lastTrackedPos;

	Agent* m_tracked;
};

