#include "GraphNode.h"


GraphNode::GraphNode() : m_shape(0.0f, 0.0f, 0.0f, 0.0f, ShapeType::BOX)
{
	m_type = NodeType::BONFIRE;
}

void GraphNode::init(int index, float x, float y, float width, float height, ShapeType type)
{
	m_shape.init(x, y, width, height, type);
	m_center.x = m_shape.getCenterX();
	m_center.y = m_shape.getCenterY();
	m_index = index;
}

void GraphNode::computeCenter()
{
	m_center.x = m_shape.getCenterX();
	m_center.y = m_shape.getCenterY();
}

void GraphNode::setPosition(glm::vec2& pos)
{
	float x = m_center.x - m_shape.destRect.x;
	float y = m_center.y - m_shape.destRect.y;
	m_center.x = pos.x + x;
	m_center.y = pos.y + y;
	m_shape.setPosition(pos);
}
void GraphNode::setSize(float offsetX, float offsetY)
{
	m_shape.destRect.z += offsetX;
	m_shape.destRect.w += offsetY;
	m_center.x += offsetX / 2.0f;
	m_center.y += offsetY / 2.0f;
}

void GraphNode::addNeighbor(int neighbor, int edge)
{
	m_neighbors[neighbor].push_back(edge);
}
