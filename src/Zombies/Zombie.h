#pragma once

#include "Agent.h"

class Human;

class Zombie : public Agent
{
public:	
	Zombie();

	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai);
	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health, float damage);

	friend std::ostream& operator<<(std::ostream& os, const Zombie& z);

	virtual void collide(Level& level, Human* entity, glm::vec2& collisionDepth) override;


	/* SETTERS */
	void setDamage(float damage){ m_damage = damage; }

	/* GETTERS */
	float getDamage(){ return m_damage; }
protected:
	float m_damage;
};

