#include "Gun.h"
#include "Player.h"
#include "Level.h"
#include "Bullet.h"

#include <ctime>
#include <random>

Gun::Gun() : Object()
{
	m_objectType = ObjectType::GUN;
	m_name = "Gun";
	m_rate = 0.60f;
	m_bulletsPerShot = 1;
	m_speed = 7.8f;
	m_spread = 0.05f;
	m_damage = 10.0f;
	m_bulletSize = glm::vec2(5.0f);
	m_pickable = true;
	m_consumable = false;
	m_loader = 5;
	m_currentLoader = m_loader;
	m_reloadSpeed = 0.2f;
	m_gunType = GunType::HANDGUN;
}

void Gun::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::Sound& effect)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
}

void Gun::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, float rate,
	int bulletPerShot, int loader, float speed, float spread, float damage, glm::vec2& bulletSize, GunType gunType)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, true);
	m_loader = m_currentLoader = loader;
	m_rate = rate;
	m_bulletsPerShot = bulletPerShot;
	m_speed = speed;
	m_spread = spread;
	m_damage = damage;
	m_bulletSize = bulletSize;
	m_gunType = gunType;
}

void Gun::draw(NS2::SpriteBatch& spriteBatch)
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

void Gun::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	debugRender.drawBox(m_shape.destRect, color, m_angle);
}

void Gun::fire(Level& level, Entity* shooter)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_spread, m_spread);

	int nbBullet = m_bulletsPerShot;
	if (nbBullet > m_currentLoader) nbBullet = m_currentLoader;
	m_currentLoader -= nbBullet;

	//m_effect.play();
	Shape& shape = shooter->getShape();
	glm::vec2& direction = shooter->getDirection();
	//glm::vec2 pos1 = shooter->getShape().getCenter() + shooter->getShape().getDimension() * direction;

	float deltaX = shape.position.x + (shape.destRect.z / 2.0f) - (m_bulletSize.x / 2.0f) +
		((shape.destRect.z / 2.0f) * direction.x);
	float deltaY = shape.position.y + (shape.destRect.z / 2.0f) - (m_bulletSize.y / 2.0f) +
		((shape.destRect.z / 2.0f) * direction.y);

	glm::vec4 pos(deltaX, deltaY, m_bulletSize.x, m_bulletSize.y);
	for (int i = 0; i < nbBullet; i++) {
		Bullet& bullet = level.getBullet();
		bullet.init(pos, glm::rotate(direction, randRotate(randomEngine)), m_damage, m_speed, shooter);
		level.toAdd.push_back(&bullet);
	}
	int d = level.getDistanceFromPlayer(shape.position);
	playEffect(d);
}
void Gun::textValue(std::string& str)
{ 
	str = m_name + " : " + std::to_string(m_currentLoader);
}