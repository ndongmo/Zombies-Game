#include "Object.h"
#include "Human.h"
#include "Level.h"

Object::Object()
{
	m_type = EntityType::OBJECT;
	m_state = EntityState::STATIC;
	m_pickable = true;
	m_onAnim = true;
}

void Object::init(float x, float y, float w, float h, ShapeType type, NS2::GLTexture& texture,
	glm::ivec2& texDims, NS2::Sound& effect)
{
	Entity::init(x, y, w, h, type, texture, texDims);
	m_effect = effect;
}

void Object::init(float x, float y, float w, float h, ShapeType type, NS2::GLTexture& texture,
	glm::ivec2& texDims, NS2::ColorRGBA8& color, glm::vec2& direction, std::string name,
	float animSpeed, NS2::Sound& effect, bool pickable)
{
	Entity::init(x, y, w, h, type, texture, texDims, color, direction, name, animSpeed);
	m_effect = effect;
	m_pickable = pickable;
}

std::ostream& operator<<(std::ostream& os, const Object& o)
{
	return os << static_cast<const Entity&>(o)
		<< "pickable[" << o.m_pickable << "])";
}

void Object::collide(Level& level, Entity* entity, glm::vec2& collisionDepth)
{
	collide(entity);
}

void Object::playEffect(int distance)
{
	if (distance < 255)
		m_effect.play(0, distance);
}

void Object::textAction(Player* player, std::string& str)
{
	if (!player->getBag().isFull())
		str = "Pick up " + m_name + " !";
	else
		str = "Your bag is full !";
}
bool Object::makeAction(Player* player, Level& level)
{
	if (!player->getBag().isFull())
	{
		if (player->addObject(this)) {
			m_alive = false;
			return true;
		}
	}
	return false;
}