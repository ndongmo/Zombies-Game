#pragma once

#include "Shape.h"

#include <unordered_map>

#define CENTER_WIDTH 5.0f

template<class T>
struct Cell;
class GraphEdge;

enum NodeType { BONFIRE, ROUNDABOUT};

class GraphNode
{
public:
	GraphNode();

	void init(int index, float x, float y, float width, float height, ShapeType type = ShapeType::BOX);
	void computeCenter();
	void addNeighbor(int neighbor, int edge);

	bool isBonfire(){ return m_type == NodeType::BONFIRE; }
	bool operator<(const GraphNode* node) const { return m_cost < node->m_cost; }

	/* SETTERS */
	void setPosition(glm::vec2& pos);
	void setSize(float offsetX, float offsetY);
	void setCenter(float x, float y){ m_center.x = x; m_center.y = y; }
	void setCell(Cell<GraphNode>* cell){ m_cell = cell; }
	void setCellIndex(int index){ m_cellIndex = index; }
	void setIndex(int index){ m_index = index; }

	/* GETTERS */
	Cell<GraphNode>* getCell(){ return m_cell; }
	int getCellIndex(){ return m_cellIndex; }
	int getIndex(){ return m_index; }
	float getCost(){ return m_cost; }
	float getWidth(){ return m_shape.destRect.z; }
	float getHeight(){ return m_shape.destRect.w; }
	const glm::vec2& getCenter(){ return m_center; }
	Shape& getShape(){ return m_shape; }
	NodeType getType(){ return m_type; }
	std::vector<int>& getEdges(int index) { return m_neighbors.at(index); }
	std::unordered_map<int, std::vector<int>>& getNeighbors() { return m_neighbors; }

private:
	int m_index;
	int m_cellIndex;
	float m_cost;
	Cell<GraphNode>* m_cell;
	Shape m_shape;
	NodeType m_type;
	glm::vec2 m_center;
	std::unordered_map<int, std::vector<int>> m_neighbors;
};

