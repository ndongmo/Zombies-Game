#pragma once

#include <vector>
#include <ctime>
#include <random>
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>

class Agent;
class Level;
class GraphNode;
class GraphEdge;
template<class T>
struct Cell;

#define SEEK_MIN_TIME 100.0f
#define SEEK_MAX_TIME 150.0f
#define SEEK_NORM_TIME 500.0f
#define SEEK_ANGLE_VIEW 1.0f

enum class AgentMove
{
	SAFEPLACE, FLEE, FOLLOW, TRACKING, LOST, EVADE, SEEKING, STAGGING, NORMAL
};

class MoveBehavior
{
public:
	void init(float lapse, int seekingPerc = 10);
	void justMoveTo(Agent* agent, glm::vec2& dir, float deltaTime);
	void moveTo(Agent* agent, glm::vec2& point, float deltaTime);
	void configSeeking(Agent* agent);
	void setBackMove();
	virtual void updateMove(Agent* agent, Level& level, float deltaTime) = 0;

	/* SETTERS */
	void setCurrentMove(AgentMove move);

	/* GETTERS */
	int getCurrentPos(){ return m_currentPos; }
	AgentMove getMoveType(){ return m_currentMove; }
	GraphEdge* getPath(){ return m_path; }

protected:
	virtual bool updateSeekingBehavior(Agent* agent);
	virtual void handleSeekingBehavior(Agent* agent, Level& level, float deltaTime);
	virtual void handleNormalBehavior(Agent* agent, Level& level, float deltaTime) = 0;

	float m_lapse;
	float m_currentTime;
	float m_moveTime;
	float m_seekTime;
	float m_currentSeekTime;
	float m_angleDir;

	int m_seekingPerc;
	int m_currentPos; /* Current path point */

	static std::mt19937 randEngine;
	glm::vec2 m_previousDir;

	AgentMove m_currentMove;
	AgentMove m_previousMove;
	GraphEdge* m_path;
};

