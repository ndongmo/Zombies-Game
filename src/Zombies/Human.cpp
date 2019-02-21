#include "Human.h"
#include "Door.h"
#include "Level.h"
#include <ResourceManager.h>

Human::Human() : Agent()
{
	m_type = EntityType::HUMAN;
	m_name = "HUMAN";
	m_invTime = 50.0f;
	m_invCurrentTime = 0.0f;
	m_invColor.setColor(255, 128, 128, 64);
	m_ammo = nullptr;
	m_weapon = nullptr;
	m_object = nullptr;
	m_idleSheet.init(NS2::ResourceManager::getTexture("Assets/humans/idle_human.png"), glm::ivec2(1, 1));
	m_moveSheet.init(NS2::ResourceManager::getTexture("Assets/humans/move_human.png"), glm::ivec2(3, 2));
	m_attackSheet.init(NS2::ResourceManager::getTexture("Assets/humans/shoot_human_1.png"), glm::ivec2(4, 2));
}

void Human::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai)
{
	Agent::init(x, y, radius, texture, texDims, ai);
}

void Human::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health)
{
	Agent::init(x, y, radius, texture, texDims, color, direction, name, animSpeed, ai, speed, health);
}

void Human::update(Level& level, float deltaTime)
{
	if (m_invCurrentTime > 0.0f) m_invCurrentTime -= deltaTime;
	m_ai->update(level, deltaTime);
}

void Human::draw(NS2::SpriteBatch& spriteBatch)
{
	if (m_tileSheet.dims.x + m_tileSheet.dims.y > 2)
	{
		m_animTime += m_animSpeed;
		if (m_animTime >= m_tileSheet.dims.x * m_tileSheet.dims.y)
		{
			m_animTime = 0.0f;
		}
	}
	if (m_invCurrentTime > 0.0f){
		spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(m_animTime), m_tileSheet.texture.id, 0.0f, m_invColor, m_direction);
	}
	else
	{
		spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(m_animTime), m_tileSheet.texture.id, 0.0f, m_color, m_direction);
	}
}

void Human::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	debugRender.drawCircle(m_shape.getCenter(), color, m_shape.getCenterWidth());
}

void Human::applyDamage(Level& level, float damage) {
	if (m_invCurrentTime <= 0.0f)
	{
		m_currentHealth -= damage;
		m_invCurrentTime = m_invTime;

		if (m_currentHealth <= 0)
		{
			m_currentHealth = 0;
			m_alive = false;
			int d = level.getDistanceFromPlayer(m_shape.position);
			NS2::ResourceManager::getSound("Sounds/pain.ogg").play(0, d);
		}
	}
}
