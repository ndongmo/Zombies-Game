#pragma once

#include "Agent.h"
#include "Gun.h"
#include "Bag.h"

class Human : public Agent
{
	friend class PathFollowingAI;

public:
	Human();

	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai);
	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health);

	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color) override;
	virtual void update(Level& level, float deltaTime) override;
	virtual void applyDamage(Level& level, float damage) override;

	void setEquipWeapon(Gun* gun){ m_weapon = gun; }
	void setEquipAmmo(Ammo* ammo){ m_ammo = ammo; }
	void setEquipObject(Object* object){ m_object = object; }

	Gun* getEquipWeapon(){ return m_weapon; }
	Ammo* geEquipAmmo(){ return m_ammo; }
	Object* getEquipObject(){ return m_object; }

protected:
	float m_invTime; // Invincible time
	float m_invCurrentTime;
	NS2::ColorRGBA8 m_invColor;

	Bag m_bag;
	Gun* m_weapon;
	Ammo* m_ammo;
	Object* m_object;
};

