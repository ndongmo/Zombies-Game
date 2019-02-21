#include "Coffer.h"
#include "Level.h"

#include <ctime>
#include <random>

Coffer::Coffer() : Object()
{
	m_objectType = ObjectType::COFFER;
	m_shape.type = ShapeType::BOX;
	m_name = "Coffer";
	m_pickable = false;
	m_health = 0.0f;
	m_texIndex = 0;
	m_destructible = false;
}

Coffer::~Coffer()
{
	Object::~Object();
	m_entities.clear();
}

void Coffer::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::Sound& effect, NS2::Sound& shootEffet)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
	m_shootEffect = shootEffet;
}

void Coffer::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed,
	NS2::Sound& effect, NS2::Sound& shootEffet, float health)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, false);
	m_shootEffect = shootEffet;
	m_health = health;
}

void Coffer::destroy(Level& level)
{
	if (m_destructible)
	{
		glm::vec2 pos = m_shape.getCenter() + (TILESIZE * m_direction);
		for (Object* ent : m_entities)
		{
			ent->getShape().setPosition(pos);
			level.toAdd.push_back(ent);
		}
		m_entities.clear();
	}
}

void Coffer::applyDamage(Level& level, float damage)
{
	if (m_destructible)
	{
		m_health -= damage;
		if (m_health <= 0)
			m_alive = false;
		int d = level.getDistanceFromPlayer(m_shape.position);
		NS2::ResourceManager::getSound("Sounds/box_breaking.ogg").play(0, d);
	}
}

void Coffer::add(Object* ent)
{
	m_entities.push_back(ent);
}

void Coffer::draw(NS2::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(m_texIndex), m_tileSheet.texture.id, 0.0f, m_color, m_direction);
}
void Coffer::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	Object::draw(debugRender, color);
}

void Coffer::textAction(Player* player, std::string& str)
{
	if (!m_destructible && m_texIndex == 0) {
		if (!player->getBag().isFull())
			str = "Open the Box !";
		else
			str = "Your bag is full !";
	}
}
bool Coffer::makeAction(Player* player, Level& level)
{
	if (!m_destructible && m_texIndex == 0)
	{
		for (size_t i = 0; i < m_entities.size(); i++) {
			if (!player->addObject(m_entities[i])) {
				level.addText(m_shape.getCenter(), "Your bag is full !");
				return true;
			}
			else {
				level.addText(m_shape.getCenter(), "+ " + m_entities[i]->getName());
				m_entities[i] = m_entities.back();
				m_entities.pop_back();
				i--;
			}
		}
		NS2::ResourceManager::getSound("Sounds/open_box.ogg").play();
		m_texIndex = 1;
		return true;
	}
	return false;
}