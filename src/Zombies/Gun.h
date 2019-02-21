#pragma once

#include "Object.h"

class Level;

enum GunType {
	RIFLE = 0,
	SHOTGUN = 1,
	HANDGUN = 2,
};
class Gun : public Object
{
public:
	Gun();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::Sound& effect);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, float rate,
		int bulletPerShot, int loader, float speed, float spread, float damage, glm::vec2& bulletSize, GunType gunType);

	virtual void fire(Level& level, Entity* shooter);
	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color) override;
	virtual void textValue(std::string& str)override;

	/* SETTERS */
	void setGunType(GunType type){ m_gunType = type; }
	void setLoader(int loader){ m_loader = loader; }
	void setRate(float rate){ m_rate = rate; }
	void setSpread(float spread){ m_spread = spread; }
	void setSpeed(float speed){ m_speed = speed; }
	void setDamage(float damage){ m_damage = damage; }
	void setBPS(int bps){ m_bulletsPerShot = bps; }
	void setBulletSize(float size){ m_bulletSize.x = m_bulletSize.y = size; }
	void setCurrentLoader(int loader){ m_currentLoader = loader; }

	/* GETTERS */
	float getReloadSpeed(){ return m_reloadSpeed; }
	int getLoader(){ return m_loader; }
	int getCurrentLoader(){ return m_currentLoader; }
	float getRate(){ return m_rate; }
	float getSpread(){ return m_spread; }
	float getSpeed(){ return m_speed; }
	int getBPS(){ return m_bulletsPerShot; }
	float getDamage(){ return m_damage; }
	float getBulletSize(){ return m_bulletSize.x; }
	GunType getGunType(){ return m_gunType; }

protected:
	int m_loader;
	int m_currentLoader;
	float m_rate; ///< Rate in term of frames
	int m_bulletsPerShot; ///< How many bullets are fired at time
	float m_spread; ///< Accuracy
	float m_speed;
	float m_damage;
	float m_reloadSpeed;
	glm::vec2 m_bulletSize;

	GunType m_gunType;
};

