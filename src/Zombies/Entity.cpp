#include "Entity.h"
#include "Level.h"
#include "Tile.h"

Entity::Entity() :
m_shape(0, 0, 0, 0, ShapeType::CIRCLE)
{
	m_color = NS2::ColorRGBA8(255, 255, 255, 255);
	m_state = EntityState::STATIC;
	m_type = EntityType::NONE;
	m_direction = glm::vec2(1, 0);
	m_name = "No NAME";
	m_animSpeed = 0.1f;
	m_angle = 0.0f;
	m_animTime = 0.0f;
	m_alive = true;
}

void Entity::init(float x, float y, float w, float h, ShapeType type, 
	NS2::GLTexture& texture, glm::ivec2& texDims)
{
	init(x, y, w, h, type, texture, texDims, NS2::ColorRGBA8(255, 255, 255, 255),
		glm::vec2(1, 0), "No NAME", 0.1f);
}

void Entity::init(float x, float y, float w, float h, ShapeType type, 
	NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed)
{
	m_shape.init(x, y, w, h, type);
	m_tileSheet.init(texture, texDims);
	m_color = color;
	m_direction = direction;
	m_name = name;
	m_animSpeed = animSpeed;
}

Entity::~Entity()
{
}

std::ostream& operator<<(std::ostream& os, const Entity& e)
{
	return os 
		<< "Entity(" 
		<< "name[" << e.m_name << "]," 
		<< e.m_shape
		<< "color[" << e.m_color.r << ", " << e.m_color.g << ", " 
			<< e.m_color.b << ", " << e.m_color.a << "],"
		<< "state[" << ((e.m_state == EntityState::DYNAMIC) ? "DYNAMIC" : "STATIC") << "], "
		<< "type[" << ((e.m_type == EntityType::BULLET) ? "BULLET" : 
			((e.m_type == EntityType::HUMAN) ? "HUMAN" :
			((e.m_type == EntityType::ZOMBIE)) ? "ZOMBIE" :
			((e.m_type == EntityType::OBJECT) ? "OBJECT" : "NONE"))) << "], "
		<< "direction[" << e.m_direction.x << ", " << e.m_direction.y << "], "
		<< "alive[" << ((e.m_alive) ? "YES" : "NO") << "], "
		;
}

void Entity::draw(NS2::SpriteBatch& spriteBatch) 
{
	if (m_tileSheet.dims.x + m_tileSheet.dims.y > 2)
	{
		m_animTime += m_animSpeed;
		if (m_animTime >= m_tileSheet.dims.x * m_tileSheet.dims.y)
		{
			m_animTime = 0.0f;
		}
	}
	
	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(m_animTime), m_tileSheet.texture.id, 0.0f, m_color, m_direction);
}

void Entity::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	if (m_shape.type == ShapeType::BOX)
	{
		debugRender.drawBox(m_shape.destRect, color, m_direction);
	}
	else
	{
		debugRender.drawCircle(m_shape.getCenter(), color, m_shape.getCenterWidth());
	}
}

void Entity::destroy(Level& level)
{
	
}


