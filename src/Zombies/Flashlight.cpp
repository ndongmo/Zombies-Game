#include "Flashlight.h"

#include "Flashlight.h"
#include "Level.h"

#define OPEN_DIFF 0.8f

Flashlight::Flashlight() : Object()
{
	m_pickable = true;
	m_consumable = true;
	m_objectType = ObjectType::FLASHLIGHT;
	m_name = "Flashlight";
	m_duration = 5000;
	m_low = 1000;
	m_on = false;
}

void Flashlight::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::Sound& effect, glm::vec2& direction)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
	m_direction = direction;
}

void Flashlight::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect,
	int duration, int low)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, false);
	m_duration = duration;
	m_low = low;
}

void Flashlight::draw(NS2::SpriteBatch& spriteBatch)
{
	if (m_onAnim)
	{
		m_angle += m_animSpeed;

		if (m_angle >= 360)
		{
			m_angle = 0;
		}
		spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_angle);
	}
	else
	{
		spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_direction);
	}
}

void Flashlight::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	Object::draw(debugRender, color);
}

void Flashlight::update(Level& level, float deltaTime)
{
	m_duration -= deltaTime;
	if (m_duration < 0.0f) m_duration = 0.0f;
}
bool Flashlight::use(Player* player, Level& level)
{
	m_on = !m_on;
	return false;
}

void Flashlight::merge(Object* obj)
{
	m_duration += static_cast<Flashlight*>(obj)->getDuration();
}