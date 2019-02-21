#pragma once

#include "Entity.h"
#include <AudioEngine.h>

class Bullet : public Entity
{
public:
	Bullet();

	void init(glm::vec4& destRect, glm::vec2& direction, float damage, float speed, Entity* pass);

	virtual bool isCrossable() override { return true; }
	virtual void update(Level& level, float deltaTime) override;
	virtual void checkCollision(Level& level) override;
	virtual void collide(Level& level, Entity* entity, glm::vec2& collisionDepth) override;
	virtual void applyDamage(Level& level, float damage) override {}

	/* GETTERS */
	float getDamage() const { return m_damage; }

protected:
	float m_damage;
	float m_speed;
	Entity* m_pass;
	NS2::Sound m_wallEffect;
};

