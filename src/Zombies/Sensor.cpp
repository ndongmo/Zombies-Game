#include "Sensor.h"
#include "Level.h"

Sensor::Sensor(AI* ai) : m_ai(ai)
{
}

void Sensor::draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8& color)
{
	for (auto& t : m_tiles)
		t->draw(debugRenderer, color);
	for (auto& z : m_zombies)
		z->draw(debugRenderer, color);
	for (auto& h : m_humans)
		h->draw(debugRenderer, color);
	for (auto& o : m_objects)
		o->draw(debugRenderer, color);
}

void Sensor::update(Agent* agent, Level& level, float deltaTime)
{
	m_currentTime += deltaTime;

	if (m_currentTime >= m_lapse)
	{
		m_tiles.clear();
		m_zombies.clear();
		m_humans.clear();
		m_objects.clear();
		m_bullets.clear();
		m_currentTime = 0.0f;

		updateSensor(agent, level, deltaTime);
	}
}