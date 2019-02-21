#include "Door.h"
#include "Player.h"
#include "Level.h"

#define OPEN_DIFF 0.8f

Door::Door() : Object()
{
	m_pickable = false;
	m_objectType = ObjectType::DOOR;
	m_name = "Door";
	m_opened = false;
	m_textureIndex = 0;
}

void Door::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, 
	NS2::Sound& effect, glm::vec2& direction)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
	m_direction = direction;
}

void Door::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, bool opened)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, false);
	m_opened = opened;
	m_textureIndex = (opened) ? 0 : 1;
}

void Door::draw(NS2::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_direction);
}

void Door::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	Object::draw(debugRender, color);
}

void Door::textAction(Player* player, std::string& str)
{
	if (player->getEquipObject() != nullptr && 
		player->getEquipObject()->getObjectType() == ObjectType::KEY)
	{
		Key* key = static_cast<Key*>(player->getEquipObject());
		if (key->getName() == m_keyName) {
			str = "Use the key " + m_keyName + " to open the door !";
			return;
		}
	}
	str = "You need the key " + m_keyName + " to open the door !";
}
bool Door::makeAction(Player* player, Level& level)
{
	if (player->getEquipObject() != nullptr &&
		player->getEquipObject()->getObjectType() == ObjectType::KEY)
	{
		Key* key = static_cast<Key*>(player->getEquipObject());
		if (key->getName() == m_keyName) {
			m_opened = true;
			m_textureIndex = 0;
			player->removeObject(key);
			m_alive = false;
			return true;
		}
	}
	return false;
}