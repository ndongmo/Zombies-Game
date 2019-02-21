#include "MigrantBehavior.h"
#include "SightSensor.h"
#include "Level.h"


void MigrantBehavior::init(float lapse, int seekingPerc)
{
	MoveBehavior::init(lapse, seekingPerc);
	m_currentPos = 0;
	m_sleepTime = 0.0f;
	m_fleeTime = 0.0f;
	m_asleep = false;
	m_path = nullptr;
}

const glm::vec2& MigrantBehavior::getCurrentPoint()
{ 
	return m_path->getPath()[m_currentPos]; 
}

GraphNode& MigrantBehavior::getNode(Level& level){
	return (m_direction < 0) ? level.nodes[m_path->getNode1()] : level.nodes[m_path->getNode2()];
}

void MigrantBehavior::updateOnStop(Agent* agent, Level& level, float deltaTime)
{
	if (m_currentMove == AgentMove::SEEKING)
		return;

	glm::vec2 pos = agent->getShape().getCenter();
	if (glm::distance(m_previousPos, pos) < STOP_DIST){
		m_stopTime += deltaTime;
		if (m_stopTime > MAX_STOP_TIME) {
			if (m_currentMove == AgentMove::NORMAL && m_path != nullptr) {
				if (m_direction == -1 && m_currentPos != m_path->getPath().size() - 1)
					m_currentPos++;
				else if (m_direction == 1 && m_currentPos != 0)
					m_currentPos--;
				else
					setCurrentMove(AgentMove::LOST);
			}
			if (m_currentMove == AgentMove::EVADE) {
				setBackMove();
			}
			m_stopTime = 0;
			m_previousPos = pos;
		}
	}
	else {
		m_stopTime = 0;
		m_previousPos = pos;
	}
}

void MigrantBehavior::turnBack()
{
	m_direction *= -1;
	if (m_direction == -1 && m_currentPos != 0){
		m_currentPos += m_direction;
	}
	else if (m_direction == 1 && m_currentPos != m_path->getPath().size() - 1){
		m_currentPos += m_direction;
	}
}

bool MigrantBehavior::getPreviousPoint(glm::vec2& pt)
{
	if (m_direction == -1 && m_currentPos != m_path->getPath().size() - 1){
		pt = m_path->getPath()[m_currentPos + 1];
		return true;
	}
	else if (m_direction == 1 && m_currentPos != 0){
		pt = m_path->getPath()[m_currentPos - 1];
		return true;
	}
	return false;
}

void MigrantBehavior::updateMove(Agent* agent, Level& level, float deltaTime)
{
	m_currentTime += deltaTime;

	if (m_currentTime >= m_lapse)
	{
		if (m_asleep){
			m_sleepTime -= deltaTime;
			if (m_sleepTime <= 0.0f)
				changePlace(level);
			return;
		}

		m_moveTime += deltaTime;

		updateOnStop(agent, level, deltaTime);

		switch (m_currentMove)
		{
		case AgentMove::NORMAL:
			handleNormalBehavior(agent, level, deltaTime);
			agent->gotoMoveMode();
			break;
		case AgentMove::LOST:
			handleLostBehavior(agent, level, deltaTime);
			agent->gotoIdleMode();
			break;
		case AgentMove::SAFEPLACE:
			handleSafePlace(agent, level, deltaTime);
			agent->gotoIdleMode();
			break;
		case AgentMove::EVADE:
			handleEvadeBehavior(agent, level, deltaTime);
			agent->gotoMoveMode();
			break;
		case AgentMove::FLEE:
			handleFleeBehavior(agent, level, deltaTime);
			agent->gotoMoveMode();
			break;
		case AgentMove::FOLLOW:
			agent->gotoMoveMode();
			break;
		case AgentMove::SEEKING:
			handleSeekingBehavior(agent, level, deltaTime);
			agent->gotoIdleMode();
			break;
		default:
			break;
		}
	}
}

void  MigrantBehavior::handleFleeBehavior(Agent* agent, Level& level, float deltaTime)
{
	m_fleeTime += deltaTime;

	if (m_fleeTime >= MAX_FLEE_TIME){
		setBackMove();
		configSeeking(agent);
	}

	glm::vec2 point = m_path->getPath()[m_currentPos];
	bool flag = Shape::circleContains(agent->getShape().getCenterX(), agent->getShape().getCenterY(), RADIUS_POINT, point);
	if (flag){
		GraphNode& node = getNode(level);
		if (((m_direction == -1 && m_currentPos == 1) ||
			(m_direction == 1 && m_currentPos == m_path->getPath().size() - 2)) &&
			node.getNeighbors().size() > 1 && node.isBonfire()){ // don't go to bonfire when on fleeing

			auto neighboor_it = node.getNeighbors().end();
			for (neighboor_it = node.getNeighbors().begin(); neighboor_it != node.getNeighbors().end(); ++neighboor_it){
				if (neighboor_it->first != m_path->getNode1() && neighboor_it->first != m_path->getNode2())
					break;
			}
			assert(neighboor_it != node.getNeighbors().end());

			std::uniform_int_distribution<int> randEdge(0, neighboor_it->second.size() - 1);
			int edge = randEdge(randEngine);
			m_path = &level.edges[neighboor_it->second.at(edge)];
			if (node.getIndex() == m_path->getNode1()) {
				m_direction = 1;
				m_currentPos = 2;
			}
			else {
				m_direction = -1;
				m_currentPos = m_path->getPath().size() - 3;
			}
		}
		else if ((m_currentPos == 0 && m_direction == -1) ||
			(m_currentPos == m_path->getPath().size() - 1 && m_direction == 1)){
			if (node.isBonfire()){
				setCurrentMove(AgentMove::SAFEPLACE);
				findFreePlace(agent, level, node);
				return;
			}
			changePlace(level);
		}
		else
		{
			m_currentPos += m_direction;
			assert(m_currentPos >= 0 && m_currentPos < m_path->getPath().size());
			point = m_path->getPath()[m_currentPos];
			moveTo(agent, point, deltaTime);
		}
	}
	else
	{
		moveTo(agent, point, deltaTime);
	}
}

void  MigrantBehavior::handleEvadeBehavior(Agent* agent, Level& level, float deltaTime)
{
	if (m_evadeFrom != nullptr){
		glm::vec2 vel;
		bool flag = Shape::simpleCollision(agent->getShape().getCenter(), agent->getShape().getCenterWidth() + EVADE_DIST,
			agent->getShape().getCenterHeight() + EVADE_DIST, m_evadeFrom->getShape().getCenter(),
			m_evadeFrom->getShape().getCenterWidth(), m_evadeFrom->getShape().getCenterHeight(), vel);

		if (flag)
		{
			vel = glm::normalize(vel);
			agent->getShape().incrPosition((vel * agent->getSpeed() * deltaTime));
			return;
		}
	}

	m_evadeFrom = nullptr;
	setBackMove();
}

void  MigrantBehavior::handleSafePlace(Agent* agent, Level& level, float deltaTime)
{
	bool flag = Shape::circleContains(agent->getShape().getCenterX(), agent->getShape().getCenterY(), 10.0f, m_currentTarget);
	bool flag1 = !(agent->getAI()->getTouchSensor().getTiles().empty() &&
		agent->getAI()->getTouchSensor().getHumans().empty() && agent->getAI()->getTouchSensor().getObjects().empty());
	if (flag || flag1){
		setBackMove();
		std::uniform_real_distribution<float> randSleep(REST_MIN_TIME, REST_MAX_TIME);
		m_sleepTime = randSleep(randEngine);
		m_asleep = true;
		m_moveTime = 0.0f;
		GraphNode& node = getNode(level);
		glm::vec2 dir = glm::normalize(node.getCenter() - agent->getShape().getCenter());
		agent->setDirection(dir);
		return;
	}
	moveTo(agent, m_currentTarget, deltaTime);
}

void  MigrantBehavior::handleLostBehavior(Agent* agent, Level& level, float deltaTime)
{
	if (!findCloserNode(agent, level)){
		SightSensor* s = &agent->getAI()->getSightSensor();
		for (size_t i = 0; i < s->getHumans().size(); i++)
		{
			MoveBehavior& mb = static_cast<HumanAI*>(s->getHumans().at(i)->getAI())->getMoveBehavior();
			if (mb.getMoveType() != AgentMove::LOST && m_path != nullptr) {
				m_path = mb.getPath();
				m_currentPos = mb.getCurrentPos();
				std::uniform_int_distribution<int> randDirection(0, 1);
				m_direction = randDirection(randEngine) == 0 ? -1 : 1;
				setCurrentMove(AgentMove::NORMAL);
				return;
			}
		}
	}
	else
	{
		setCurrentMove(AgentMove::NORMAL);
	}

	glm::vec2& dir = agent->getDirection();

	if (!agent->getAI()->getTouchSensor().getTiles().empty()) {
		std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
		dir.x = randDir(randEngine);
		dir.y = randDir(randEngine);
	}

	glm::vec2 point = agent->getShape().getCenter() + (dir * TILESIZE);
	moveTo(agent, point, deltaTime);
}

void  MigrantBehavior::handleNormalBehavior(Agent* agent, Level& level, float deltaTime)
{
	if (updateSeekingBehavior(agent)) return;

	if (m_path == nullptr && m_lastPath.empty()) {
		if (!findCloserNode(agent, level)){
			m_moveTime = 0.0f;
			setCurrentMove(AgentMove::LOST);
		}
	}
	else if (m_path == nullptr && !m_lastPath.empty()) {
		followLastPath(agent, level, deltaTime);
	}
	else {
		glm::vec2 point = m_path->getPath()[m_currentPos];
		bool flag = Shape::circleContains(agent->getShape().getCenterX(), agent->getShape().getCenterY(), RADIUS_POINT, point);
		if (flag){
			if ((m_currentPos == 0 && m_direction == -1) ||
				(m_currentPos == m_path->getPath().size() - 1 && m_direction == 1)){

				assert(m_path->getNode1() >= 0 && m_path->getNode1() < level.nodes.size());
				assert(m_path->getNode2() >= 0 && m_path->getNode2() < level.nodes.size());

				GraphNode& node = getNode(level);
				if (node.isBonfire()){
					setCurrentMove(AgentMove::SAFEPLACE);
					findFreePlace(agent, level, node);
					return;
				}
				changePlace(level);
			}
			else
			{
				m_currentPos += m_direction;
				assert(m_currentPos >= 0 && m_currentPos < m_path->getPath().size());
				point = m_path->getPath()[m_currentPos];
				moveTo(agent, point, deltaTime);
			}
		}
		else
		{
			moveTo(agent, point, deltaTime);
		}
	}
}

void MigrantBehavior::changePlace(Level& level)
{
	assert(m_path->getNode1() >= 0 && m_path->getNode1() < level.nodes.size());
	assert(m_path->getNode2() >= 0 && m_path->getNode2() < level.nodes.size());

	GraphNode& node = getNode(level);
	assert(node.getNeighbors().size() > 0);

	std::uniform_int_distribution<int> randPath(0, node.getNeighbors().size() - 1);
	int neighboor = randPath(randEngine);
	auto neighboor_it = std::next(std::begin(node.getNeighbors()), neighboor);

	std::uniform_int_distribution<int> randEdge(0, neighboor_it->second.size() - 1);
	int edge = randEdge(randEngine);
	m_path = &level.edges[neighboor_it->second.at(edge)];
	if (node.getIndex() == m_path->getNode1()) {
		m_direction = 1;
		m_currentPos = 0;
	}
	else {
		m_direction = -1;
		m_currentPos = m_path->getPath().size() - 1;
	}
	m_asleep = false;
	m_sleepTime = 0.0f;
}

void MigrantBehavior::followLastPath(Agent* agent, Level& level, float deltaTime)
{
	assert(m_lastPath.size() > 0);

	if (agent->getShape().contains(m_lastPath.back())){
		m_lastPath.pop_back();
	}
	if (m_lastPath.empty()){
		glm::vec2& pos = agent->getShape().getCenter();
		Cell<GraphNode>* cell = nullptr;
		int x, y;
		bool flag = false;
		if (level.nodeGrid.isCell(pos, x, y))
		{
			cell = &level.nodeGrid.getCell(x, y);
			flag = canReachNode(level, pos, cell, &m_path, m_currentPos);
			if (flag)
			{
				std::uniform_int_distribution<int> randDirection(0, 1);
				m_direction = randDirection(randEngine) == 0 ? -1 : 1;
			}
		}
	}
	else
	{
		moveTo(agent, m_lastPath.back(), deltaTime);
	}
}

bool MigrantBehavior::findCloserNode(Agent* agent, Level& level)
{
	glm::vec2& pos = agent->getShape().getCenter();
	Cell<GraphNode>* cell = nullptr;
	int x, y;
	bool flag = false;
	const int MAX_ITERATION = 3; /* In order to not spend a lot of time for searching path */

	if (level.nodeGrid.isCell(pos, x, y))
	{
		cell = &level.nodeGrid.getCell(x, y);
		flag = canReachNode(level, pos, cell, &m_path, m_currentPos);
	}
	if (!flag)
	{
		bool left = true, right = true, top = true, bottom = true;
		glm::vec2 tmp;
		for (unsigned int i = 1; (left || right || top || bottom) && i < MAX_ITERATION; i++)
		{
			if (left) {
				tmp.x = pos.x - level.nodeGrid.getCellSize() * i;
				left = level.nodeGrid.isCell(tmp, x, y);
				if (left) {
					cell = &level.nodeGrid.getCell(x, y);
					flag = canReachNode(level, pos, cell, &m_path, m_currentPos);
					if (flag) break;
				}
			}
			if (right) {
				tmp.x = pos.x + level.nodeGrid.getCellSize() * i;
				right = level.nodeGrid.isCell(tmp, x, y);
				if (right) {
					cell = &level.nodeGrid.getCell(x, y);
					flag = canReachNode(level, pos, cell, &m_path, m_currentPos);
					if (flag) break;
				}
			}
			if (top) {
				tmp.y = pos.y + level.nodeGrid.getCellSize() * i;
				top = level.nodeGrid.isCell(tmp, x, y);
				if (top) {
					cell = &level.nodeGrid.getCell(x, y);
					flag = canReachNode(level, pos, cell, &m_path, m_currentPos);
					if (flag) break;
				}
			}
			if (bottom) {
				tmp.y = pos.y - level.nodeGrid.getCellSize() * i;
				bottom = level.nodeGrid.isCell(tmp, x, y);
				if (bottom) {
					cell = &level.nodeGrid.getCell(x, y);
					flag = canReachNode(level, pos, cell, &m_path, m_currentPos);
					if (flag) break;
				}
			}
		}
	}

	if (flag) {
		std::uniform_int_distribution<int> randDirection(0, 1);
		m_direction = randDirection(randEngine) == 0 ? -1 : 1;
	}

	return flag;
}

bool MigrantBehavior::canReachNode(Level& level, glm::vec2& pos, Cell<GraphNode>* cell, GraphEdge** edge, int& point)
{
	for (GraphNode* n : cell->entities) { // go through nodes
		for (auto& ne : n->getNeighbors()) { // go through neighboors
			for (auto& e : ne.second) { // go through edges
				assert(e >= 0 && e < level.edges.size());
				for (unsigned int i = 0; i < level.edges[e].getPath().size(); i++) { // go through edge points
					const glm::vec2& pathPoint = level.edges[e].getPath()[i];
					if (canReachPoint(level, pos, pathPoint)) {
						*edge = &level.edges[e];
						point = i;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool MigrantBehavior::canReachPoint(Level& level, glm::vec2& pos, const glm::vec2& point)
{
	glm::vec2 dir = glm::normalize(point - pos);
	glm::vec2 tilePoint = pos;
	bool flag1 = false, flag2 = false;
	do
	{
		tilePoint += TILESIZE * dir;
		Tile* t = level.getTile((int)(tilePoint.x / TILESIZE), (int)(tilePoint.y / TILESIZE));
		flag1 = t != nullptr && t->isCrossable();
		flag2 = glm::distance(pos, tilePoint) < glm::distance(pos, point);
	} while (flag2 && flag1);

	return (flag1 || (!flag1 && !flag2));
}

bool MigrantBehavior::findFreePlace(Agent* agent, Level& level, GraphNode& node)
{
	assert((m_currentPos == 0 && m_direction == -1) ||
		(m_currentPos == m_path->getPath().size() - 1 && m_direction == 1));

	std::uniform_real_distribution<float> randX(node.getShape().destRect.x,
		node.getShape().destRect.x + node.getShape().destRect.z - agent->getShape().getCenterWidth());
	std::uniform_real_distribution<float> randY(node.getShape().destRect.y,
		node.getShape().destRect.y + node.getShape().destRect.w - agent->getShape().getCenterHeight());

	m_currentTarget.x = randX(randEngine);
	m_currentTarget.y = randY(randEngine);
	return false;
}