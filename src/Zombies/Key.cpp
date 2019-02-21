#include "Key.h"
#include "Player.h"
#include "Level.h"

Key::Key() : Object()
{
	m_objectType = ObjectType::KEY;
	m_name = "Key";
	m_pickable = true;
	m_consumable = true;
}

void Key::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::Sound& effect)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
}

void Key::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, true);
}

void Key::draw(NS2::SpriteBatch& spriteBatch)
{
	m_angle += m_animSpeed;

	if (m_angle >= 360)
	{
		m_angle = 0;
	}

	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_angle);
}

void Key::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	debugRender.drawBox(m_shape.destRect, color, m_angle);
}
bool Key::use(Player* player, Level& level)
{
	if (player->getInterAction() != nullptr) {
		Door* door = dynamic_cast<Door*>(player->getInterAction());
		if (door != nullptr && door->getKeyName() == m_name) {
			NS2::ResourceManager::getSound("Sounds/key_grab.ogg").play();
			door->setOpened(true);
			door->setAlive(false);
			m_alive = false;
			return true;
		}
	}
	return false;
}

