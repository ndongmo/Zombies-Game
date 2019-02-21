#pragma once

#include "Tile.h"

#include <vector>
#include <glm\glm.hpp>
#include <DebugRenderer.h>

#define MIN_DISTANCE 5.0f
#define MAX_DISTANCE 500.0f

class GraphNode;

class GraphEdge
{
	friend class MapGraph;

public:
	GraphEdge();
	GraphEdge(GraphNode* node1, GraphNode* node2, std::vector<glm::vec2>& path, int width);
	~GraphEdge();
	
	void reset();
	void draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8& color);
	void startPath(int node1, const glm::vec2& point);
	void endPath(int node2);
	void removeLastPoint();
	bool addPathPoint(const glm::vec2& point);
	void updateLastPoint(const glm::vec2& point);

	/* SETTERS */
	void setNode1(int node1){ m_node1 = node1; }
	void setNode2(int node2){ m_node2 = node2; }
	void setWidth(float width){ m_width = width; }
	void setCost(float cost){ m_cost = cost; }
	void setIndex(int index){ m_index = index; }

	/* GETTERS */
	int getNode1(){ return m_node1; }
	int getNode2(){ return m_node2; }
	unsigned int getSize(){ return m_path.size(); }
	float getWidth(){ return m_width; }
	float getRadius(){ return m_width / 2.0f; }
	float getCost(){ return m_cost; }

	std::vector<glm::vec2>& getPath() { return m_path; }
	GraphNode* getStart(){ return nullptr; }

private:
	float m_width;
	float m_cost;
	int m_index;
	int m_node1;
	int m_node2;
	std::vector<glm::vec2> m_path;
};

