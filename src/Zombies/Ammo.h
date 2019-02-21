#pragma once

#include "Object.h"
#include "Gun.h"

#include <glm\glm.hpp>

class Ammo : public Object
{
public:
	Ammo();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::Sound& effect, glm::vec2& direction);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed,
		NS2::Sound& effect, int amount, GunType gunType);

	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color) override;
	virtual bool isMergeable()override{ return true; }
	virtual bool canBeMerged(Object* obj)override;
	virtual void merge(Object* obj)override;

	void setGunType(GunType type){ m_gunType = type; }
	void setAmount(int amount) { m_amount = amount; }
	void decreaseAmount(int amount) { m_amount -= amount; }
	void increaseAmount(int amount){ m_amount += amount; }
	int getAmount() { return m_amount; }
	GunType getGunType(){ return m_gunType; }

private:
	unsigned int m_amount;
	GunType m_gunType;
};

