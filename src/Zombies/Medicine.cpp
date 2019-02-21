#include "Medicine.h"
#include "Player.h"
#include "Level.h"

Medicine::Medicine() : Object()
{
	m_objectType = ObjectType::MEDICINE;
	m_name = "Medikit";
	m_health = 0.0f;
	m_consumable = true;
}

void Medicine::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::Sound& effect)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
}

void Medicine::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, float health)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, true);
	m_health = health;
}

void Medicine::draw(NS2::SpriteBatch& spriteBatch)
{
	m_angle += m_animSpeed;
	
	if (m_angle >= 360)
	{
		m_angle = 0;
	}

	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_angle);
}

void Medicine::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	debugRender.drawBox(m_shape.destRect, color, m_angle);
}

bool Medicine::use(Player* player, Level& level)
{
	player->applyHealth(m_health);
	m_alive = false;
	return true;
}