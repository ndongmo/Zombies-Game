#include "Zombie.h"
#include "Human.h"
#include "Level.h"

#include <ResourceManager.h>

Zombie::Zombie() : Agent()
{
	m_type = EntityType::ZOMBIE;
	m_name = "ZOMBIE";
	m_damage = 10.0f;
	m_idleSheet.init(NS2::ResourceManager::getTexture("Assets/zombies/skeleton-idle.png"), glm::ivec2(5, 3));
	m_moveSheet.init(NS2::ResourceManager::getTexture("Assets/zombies/skeleton-move.png"), glm::ivec2(5, 3));
	m_attackSheet.init(NS2::ResourceManager::getTexture("Assets/zombies/skeleton-attack.png"), glm::ivec2(4, 2));
}

void Zombie::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai)
{
	Agent::init(x, y, radius, texture, texDims, ai);
}

void Zombie::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health, float damage)
{
	Agent::init(x, y, radius, texture, texDims, color, direction, name, animSpeed, ai, speed, health);
	m_damage = damage;
}

std::ostream& operator<<(std::ostream& os, const Zombie& z)
{
	return os << static_cast<const Agent&>(z)
		<< "damage[" << z.m_damage << "])";
}

void Zombie::collide(Level& level, Human* entity, glm::vec2& collisionDepth)
{
	if (entity->isAlive()) {
		
		entity->applyDamage(level, m_damage);
		if (!entity->isAlive())
			level.transformToZombie(entity);
	}
}
