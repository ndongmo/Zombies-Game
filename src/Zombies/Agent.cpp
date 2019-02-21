#include "Agent.h"
#include "LevelHandler.h"
#include "Tile.h"

#include <algorithm>
#include <glm\gtx\rotate_vector.hpp>

Agent::Agent() : Entity()
{
	m_speed = 1.0f;
	m_currentHealth = m_health = 100.0f;
	m_frameAction = 20.0f;
	m_currentFrameAction = 0.0f;
	m_state = EntityState::DYNAMIC;
	m_onAnim = false;
	m_animMode = AnimMode::ANIM_IDLE;
	m_angularSpeed = 0.1f;
}

Agent::~Agent()
{
	m_ai = nullptr;
}

void Agent::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai)
{
	Entity::init(x, y, radius, radius, ShapeType::CIRCLE, texture, texDims);
	m_ai = ai;
}

void Agent::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health)
{
	Entity::init(x, y, radius, radius, ShapeType::CIRCLE, texture, texDims, color, direction, name, animSpeed);
	m_ai = ai;
	m_speed = speed;
	m_currentHealth = m_health = health;
}

void Agent::endInit()
{
	m_tileSheet = m_idleSheet;
	m_animTime = 0.0f;
	m_animMode = AnimMode::ANIM_IDLE;
}

std::ostream& operator<<(std::ostream& os, const Agent& a)
{
	return os << static_cast<const Entity&>(a)
		<< "AI [" << a.m_ai->getName() << "], "
		<< "speed[" << a.m_speed << "], "
		<< "health[" << a.m_currentHealth << "]"
		;
}

void Agent::setAI(AI* ai)
{
	m_ai = ai;
}

void Agent::applyDamage(Level& level, float damage) {
	m_currentHealth -= damage;
	if (m_currentHealth <= 0)
	{
		m_currentHealth = 0;
		m_alive = false;
	}
}

void Agent::applyHealth(float health)
{
	m_currentHealth += health;
	if (m_health < m_currentHealth)
		m_currentHealth = m_health;
}

void Agent::gotoIdleMode()
{ 
	if (m_animMode != AnimMode::ANIM_IDLE)
	{
		m_tileSheet = m_idleSheet; 
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_IDLE;
	}
}
void Agent::gotoMoveMode()
{ 
	if (m_animMode != AnimMode::ANIM_MOVE)
	{
		m_tileSheet = m_moveSheet;
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_MOVE;
	}
}
void Agent::gotoAttackMode()
{ 
	if (m_animMode != AnimMode::ANIM_ATTACK)
	{
		m_tileSheet = m_attackSheet;
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_ATTACK;
	}
}

void Agent::collide(Level& level, Entity* entity, glm::vec2& collisionDepth)
{
	if (entity->getType() == EntityType::HUMAN){
		Human* h = static_cast<Human*>(entity);
		m_ai->getTouchSensor().getHumans().push_back(h);
		m_ai->getTouchSensor().notifyForHuman();
		collide(level, h, collisionDepth);
	}
	else if (entity->getType() == EntityType::ZOMBIE){
		Zombie* z = static_cast<Zombie*>(entity);
		m_ai->getTouchSensor().getZombies().push_back(z);
		m_ai->getTouchSensor().notifyForZombie();
	}
	else if (entity->getType() == EntityType::BULLET){
		Bullet* b = static_cast<Bullet*>(entity);
		m_ai->getTouchSensor().getBullets().push_back(b);
		m_ai->getTouchSensor().notifyForBullet();
	}
	else if (entity->getType() == EntityType::OBJECT){
		Object* o = static_cast<Object*>(entity);
		m_ai->getTouchSensor().getObjects().push_back(o);
		m_ai->getTouchSensor().notifyForObject();
		if (o->isCrossable()) return;
	}

	m_shape.incrPosition(collisionDepth.x, collisionDepth.y);
}

void Agent::checkCollision(Level& level)
{
	int x = floor(m_shape.destRect.x / TILESIZE);
	int y = floor(m_shape.destRect.y / TILESIZE);
	int xlength = floor((m_shape.destRect.x + m_shape.destRect.z) / TILESIZE);
	int ylength = floor((m_shape.destRect.y + m_shape.destRect.w) / TILESIZE);

	for (unsigned int i = x; i <= xlength; i++)
	{
		for (unsigned int j = y; j <= ylength; j++)
		{
			Tile* t = level.getTile(i, j);
			if (t != nullptr && !t->isCrossable())
			{
				if (m_shape.collideTile(t->getCenter(), TILESIZE)){
					m_ai->getTouchSensor().getTiles().push_back(t);
					m_ai->getTouchSensor().notifyForTile();
				}
			}
		}
	}

}

void Agent::rotate(Level& level, glm::vec2& pt, float deltatime)
{
	glm::vec2 dir = glm::normalize(pt - m_shape.getCenter());
	float angle = atan2f(dir.y, dir.x) - atan2f(m_direction.y, m_direction.x);

	if (angle < -M_PI) angle += M_PI * 2;
	if (angle > M_PI) angle -= M_PI * 2;
	
	dir = glm::rotate(m_direction, angle * deltatime * m_angularSpeed);
	m_direction = glm::normalize(dir);
}
void Agent::walk(Level& level)
{

}
bool Agent::canWalk()
{
	return true;
}