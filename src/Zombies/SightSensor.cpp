#include "SightSensor.h"
#include "Level.h"
#include "AI.h"

#include <glm\gtx\rotate_vector.hpp>
#include <algorithm>

SightSensor::SightSensor(AI* ai) : Sensor(ai)
{
}

void SightSensor::init(float radius, float angle, float lapse)
{
	m_radius = radius;
	m_angle = glm::radians(angle);
	m_lapse = lapse;
	m_currentTime = 0.0f;
}

void SightSensor::draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8& color)
{
	debugRenderer.drawLine(m_center, m_left, color);
	debugRenderer.drawLine(m_center, m_right, color);
	debugRenderer.drawLine(m_left, m_right, color);

	Sensor::draw(debugRenderer, color);
}

void SightSensor::notifyForTile()
{
	m_ai->sightNotifyForTile();
}
void SightSensor::notifyForZombie()
{
	m_ai->sightNotifyForZombie();
}
void SightSensor::notifyForHuman()
{
	m_ai->sightNotifyForHuman();
}
void SightSensor::notifyForObject()
{
	m_ai->sightNotifyForObject();
}

void SightSensor::notifyForBullet()
{
	m_ai->sightNotifyForBullet();
}

/*
	First we get the triangle using agent direction.
	Then we get the square around the triangle.
	Afterward, we loop through the cells in the square.
	For every cell, if it's in the triangle, then we
	can test each entity if it's in the triangle and if it isn't hided by wall.
	*/
void SightSensor::updateSensor(Agent* agent, Level& level, float deltaTime)
{
	m_center = agent->getShape().getCenter();
	m_left = m_center + glm::rotate(agent->getDirection(), m_angle / 2.0f) * m_radius;
	m_right = m_center + glm::rotate(agent->getDirection(), -m_angle / 2.0f) * m_radius;

	float minX = (m_center.x < m_left.x) ? std::min(m_center.x, m_right.x) : std::min(m_left.x, m_right.x);
	float minY = (m_center.y < m_left.y) ? std::min(m_center.y, m_right.y) : std::min(m_left.y, m_right.y);
	float maxX = (m_center.x > m_left.x) ? std::max(m_center.x, m_right.x) : std::max(m_left.x, m_right.x);
	float maxY = (m_center.y > m_left.y) ? std::max(m_center.y, m_right.y) : std::max(m_left.y, m_right.y);

	float cellSize = level.grid.getCellSize();
	unsigned int width = ceil((maxX - minX) / cellSize) + 1;
	unsigned int height = ceil((maxY - minY) / cellSize) + 1;
	glm::vec2 point;

	/* find where i'm */
	Cell<Entity>& cell = level.grid.getCell(m_center);
	find(agent, level, cell);

	for (unsigned int j = 0; j < height; j++)
	{
		point.y = minY + (j * cellSize);
		for (unsigned int i = 0; i < width; i++)
		{
			point.x = minX + (i * cellSize);
			int cellX;
			int cellY;
			if (level.grid.isCell(point, cellX, cellY))
			{
				Cell<Entity>& cell = level.grid.getCell(point);
				bool b = false;
				if (Shape::pointIntriangle(glm::vec2(cellX * cellSize, cellY * cellSize), m_center, m_left, m_right)) {
					b = true;
				}
				else if (Shape::pointIntriangle(glm::vec2((cellX * cellSize) + cellSize, cellY * cellSize), m_center, m_left, m_right)) {
					b = true;
				}
				else if (Shape::pointIntriangle(glm::vec2(cellX * cellSize, (cellY * cellSize) + cellSize), m_center, m_left, m_right)) {
					b = true;
				}
				else if (Shape::pointIntriangle(glm::vec2((cellX * cellSize) + cellSize, (cellY * cellSize) + cellSize), m_center, m_left, m_right)) {
					b = true;
				}

				if (b) find(agent, level, cell);
			}
		}
	}
}

void SightSensor::find(Agent* agent, Level& level, Cell<Entity>& cell)
{
	for (Entity* e : cell.entities){
		if (e == agent) continue;
		bool b = false;
		glm::vec2& detectPoint = e->getShape().getBL();
		if (Shape::pointIntriangle(detectPoint, m_center, m_left, m_right)) {
			b = true;
		}
		else if (Shape::pointIntriangle(detectPoint = e->getShape().getBR(), m_center, m_left, m_right)) {
			b = true;
		}
		else if (Shape::pointIntriangle(detectPoint = e->getShape().getTR(), m_center, m_left, m_right)) {
			b = true;
		}
		else if (Shape::pointIntriangle(detectPoint = e->getShape().getTL(), m_center, m_left, m_right)) {
			b = true;
		}

		if (!b) continue;

		glm::vec2 dir = glm::normalize(detectPoint - m_center);
		glm::vec2 tilePoint = m_center;
		bool b2 = false;
		do
		{
			tilePoint += TILESIZE * dir;
			Tile* t = level.getTile((int)(tilePoint.x / TILESIZE), (int)(tilePoint.y / TILESIZE));
			b = t != nullptr && t->isCrossable();
			b2 = glm::distance(m_center, tilePoint) < glm::distance(m_center, detectPoint);
		} while (b2 && b);

		if (b || (!b && !b2))
		{
			if (e->getType() == EntityType::HUMAN)
				m_humans.push_back(static_cast<Human*>(e));
			else if (e->getType() == EntityType::ZOMBIE)
				m_zombies.push_back(static_cast<Zombie*>(e));
			else if (e->getType() == EntityType::OBJECT)
				m_objects.push_back(static_cast<Object*>(e));
			else if (e->getType() == EntityType::BULLET)
				m_bullets.push_back(static_cast<Bullet*>(e));
		}
	}
}