#include "Bullet.h"
#include "Level.h"

#include <Vertex.h>

Bullet::Bullet(): Entity()
{
	Entity::init(0.0f, 0.0f, 20.0f, 20.0f, ShapeType::CIRCLE, 
		NS2::ResourceManager::getTexture("Assets/circle.png"), glm::ivec2(1));
	m_wallEffect = NS2::ResourceManager::getSound("Sounds/wall_ricochet_1.ogg");
	m_type = EntityType::BULLET;
	m_state = EntityState::DYNAMIC;
	m_name = "Bullet";
	m_alive = false;
	m_color.setColor(64, 64, 64, 255);
}

void Bullet::init(glm::vec4& destRect, glm::vec2& direction, float damage, float speed, Entity* pass)
{
	m_tileSheet.init(NS2::ResourceManager::getTexture("Assets/bullet.png"), glm::ivec2(1));
	m_color.setColor(255, 0, 0, 255);
	m_shape.setDestRect(destRect);
	m_direction = direction;
	m_damage = damage;
	m_speed = speed;
	m_alive = true;
	m_pass = pass;
}

void Bullet::update(Level& level, float deltaTime)
{
	if (m_alive)
	{
		m_shape.incrPosition(m_direction * m_speed * deltaTime);
	}
}

void Bullet::collide(Level& level, Entity* entity, glm::vec2& collisionDepth)
{
	if (!entity->isCrossable() && entity != m_pass)
	{
		entity->applyDamage(level, m_damage);
		m_alive = false;
		if (entity->getType() == EntityType::HUMAN || entity->getType() == EntityType::ZOMBIE) {
			level.addBlood(entity->getShape().getCenter(), 5);
			level.addText(entity->getShape().getCenter(), std::to_string((int)m_damage));
			NS2::ResourceManager::getSound("Sounds/small_pain.ogg").play();
		}
	}
}

void Bullet::checkCollision(Level& level)
{
	int x = floor(m_shape.destRect.x / TILESIZE);
	int y = floor(m_shape.destRect.y / TILESIZE);
	int xlength = floor((m_shape.destRect.x + m_shape.destRect.z) / TILESIZE);
	int ylength = floor((m_shape.destRect.y + m_shape.destRect.w) / TILESIZE);

	for (unsigned int i = x; i <= xlength && i < level.width; i++)
	{
		for (unsigned int j = y; j <= ylength && j < level.height; j++)
		{
			Tile* t = level.getTile(i, j);
			if (t != nullptr && !t->isCrossable())
			{
				if (m_shape.collideTile(t->getCenter(), TILESIZE))
				{
					m_alive = false;
					int d = level.getDistanceFromPlayer(m_shape.position);
					if (d < 255) m_wallEffect.play(0, d);
					return;
				}
			}
		}
	}
}