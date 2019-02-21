#include "GraphEdge.h"


GraphEdge::GraphEdge()
{
	m_width = 10.0f;
	m_cost = 0.0f;
	m_node1 = -1;
	m_node2 = -1;
}

GraphEdge::GraphEdge(GraphNode* node1, GraphNode* node2, std::vector<glm::vec2>& path, int width) :
m_path(path), m_width(width)
{
}


GraphEdge::~GraphEdge()
{
}

void GraphEdge::updateLastPoint(const glm::vec2& point)
{
	if (!m_path.empty()) {
		m_path.back().x = point.x;
		m_path.back().y = point.y;
	}
}

void GraphEdge::startPath(int node1, const glm::vec2& point)
{
	reset();
	m_node1 = node1;
	m_path.push_back(point);
	m_path.push_back(point); // mouse point
}
void GraphEdge::endPath(int node2)
{
	m_node2 = node2;
	m_path.pop_back(); // remove mouse point
}
void GraphEdge::removeLastPoint()
{
	if (m_path.size() > 2)
		m_path.pop_back();
}
bool GraphEdge::addPathPoint(const glm::vec2& point)
{
	if (m_path.size() > 1) {
		float dis = glm::distance(m_path[m_path.size() - 2], point);
		if (dis > MIN_DISTANCE && dis < MAX_DISTANCE){
			m_cost += dis;
			m_path.back().x = point.x;
			m_path.back().y = point.y;
			m_path.push_back(point);
			return true;
		}
	}
	
	return false;
}

void GraphEdge::reset(){
	m_cost = 0.0f;
	m_width = 10.0f;
	m_node1 = -1;
	m_node2 = -1;
	m_path.clear();
}

void GraphEdge::draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8& color)
{
	float radius = getRadius();

	for (size_t w = 1; w < m_path.size(); w++)
	{
		glm::vec2 dir = glm::normalize(m_path[w] - m_path[w - 1]);
		glm::vec2 dirPrevious;
		glm::vec2 dirNext;

		if (w >= 2)
			dirPrevious = glm::normalize(m_path[w - 1] - m_path[w - 2]);
		if (w + 1 < m_path.size())
			dirNext = glm::normalize(m_path[w + 1] - m_path[w]);

		glm::vec2 p11(m_path[w - 1].x - (radius * dir.y) - (radius * dirPrevious.y),
			m_path[w - 1].y + (radius * dir.x) + (radius * dirPrevious.x));
		glm::vec2 p12(m_path[w - 1].x + (radius * dir.y) + (radius * dirPrevious.y),
			m_path[w - 1].y - (radius * dir.x) - (radius * dirPrevious.x));
		glm::vec2 p21(m_path[w].x - (radius * dir.y) - (radius * dirNext.y),
			m_path[w].y + (radius * dir.x) + (radius * dirNext.x));
		glm::vec2 p22(m_path[w].x + (radius * dir.y) + (radius * dirNext.y),
			m_path[w].y - (radius * dir.x) - (radius * dirNext.x));


		debugRenderer.drawLine(m_path[w - 1], m_path[w], color);
		debugRenderer.drawLine(p11, p21, color);
		debugRenderer.drawLine(p12, p22, color);
	}
}