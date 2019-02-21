#include "Ammo.h"

Ammo::Ammo() : Object()
{
	m_pickable = true;
	m_consumable = false;
	m_objectType = ObjectType::AMMO;
	m_name = "Ammo";
	m_amount = 10;
}

void Ammo::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::Sound& effect, glm::vec2& direction)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
	m_direction = direction;
}

void Ammo::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect,
	int amount, GunType gunType)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, false);
	m_amount = amount;
	m_gunType = gunType;
}

void Ammo::draw(NS2::SpriteBatch& spriteBatch)
{
	m_angle += m_animSpeed;

	if (m_angle >= 360)
	{
		m_angle = 0;
	}

	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_angle);
}

void Ammo::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	debugRender.drawBox(m_shape.destRect, color, m_angle);
}

bool Ammo::canBeMerged(Object* obj)
{
	return static_cast<Ammo*>(obj)->getGunType() == m_gunType;
}
void Ammo::merge(Object* obj)
{
	m_amount += static_cast<Ammo*>(obj)->getAmount();
}