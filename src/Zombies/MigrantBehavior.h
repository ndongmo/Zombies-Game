#pragma once

#include "MoveBehavior.h"

#define RADIUS_POINT 10.0f
#define EVADE_DIST 5.0f
#define STOP_DIST 1.0f
#define REST_MIN_TIME 200
#define REST_MAX_TIME 500
#define MAX_FLEE_TIME 200
#define MAX_STOP_TIME 100

class MigrantBehavior : public MoveBehavior
{
public:
	void init(float lapse, int seekingPerc = 10);
	void turnBack();
	virtual void updateMove(Agent* agent, Level& level, float deltaTime) override;
	
	/* SETTERS */
	void setEvadeFrom(Agent* a){ m_evadeFrom = a; }
	void setFleeFrom(Agent* a){ m_fleeFrom = a; }

	/* GETTERS */
	bool isSleeping(){ return m_asleep; }
	const glm::vec2& getCurrentPoint();
	bool getPreviousPoint(glm::vec2& pt);
	Agent* getEvadeFrom(){ return m_evadeFrom; }
	Agent* getFleeFrom(){ return m_fleeFrom; }
	GraphNode& getNode(Level& level);

protected:
	void updateOnStop(Agent* agent, Level& level, float deltaTime);
	void handleFleeBehavior(Agent* agent, Level& level, float deltaTime);
	void handleEvadeBehavior(Agent* agent, Level& level, float deltaTime);
	void handleSafePlace(Agent* agent, Level& level, float deltaTime);
	void handleLostBehavior(Agent* agent, Level& level, float deltaTime);
	virtual void handleNormalBehavior(Agent* agent, Level& level, float deltaTime) override;

	bool findCloserNode(Agent* agent, Level& level);
	bool findFreePlace(Agent* agent, Level& level, GraphNode& node);
	bool canReachNode(Level& level, glm::vec2& pos, Cell<GraphNode>* cell, GraphEdge** edge, int& point);
	bool canReachPoint(Level& level, glm::vec2& pos, const glm::vec2& point);
	void followLastPath(Agent* agent, Level& level, float deltaTime);
	void changePlace(Level& level);

	float m_sleepTime;
	float m_stopTime;
	float m_fleeTime;
	float m_angleDir;

	int m_direction;

	bool m_asleep;

	glm::vec2 m_currentTarget;
	glm::vec2 m_previousPos;
	std::vector<glm::vec2> m_lastPath; /* Last saved path */

	Agent* m_fleeFrom;
	Agent* m_evadeFrom;
};

