#pragma once

#include "Entity.h"
#include "AI.h"
#include "Gun.h"
#include "Door.h"
#include "Ammo.h"

enum AnimMode{ ANIM_IDLE, ANIM_MOVE, ANIM_ATTACK, ANIM_RELOAD };

class Agent : public Entity
{
public:
	Agent();
	~Agent();

	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai);
	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health);

	friend std::ostream& operator<<(std::ostream& os, const Agent& a);

	virtual void update(Level& level, float deltaTime){ m_ai->update(level, deltaTime); }
	virtual void collide(Level& level, Entity* entity, glm::vec2& collisionDepth) override;
	virtual void checkCollision(Level& level) override;
	virtual void applyDamage(Level& level, float damage) override;
	virtual void collide(Level& level, Human* entity, glm::vec2& collisionDepth){}
	virtual void applyHealth(float health);
	virtual void endInit()override;

	virtual void gotoIdleMode();
	virtual void gotoMoveMode();
	virtual void gotoAttackMode();

	void rotate(Level& level, glm::vec2& pt, float deltatime);
	void walk(Level& level);
	bool canWalk();

	/* SETTERS */
	void setAI(AI* ai);
	void setHealth(float health){ m_health = m_currentHealth = health; }
	void setSpeed(float speed){ m_speed = speed; }
	
	/* GETTERS */
	AI* getAI(){ return m_ai; }
	float getInitHealth() { return m_health; }
	float getHealth() const { return m_currentHealth; }
	float getSpeed() { return m_speed; }
	
protected:
	float m_speed;
	float m_health;
	float m_currentHealth;
	float m_frameAction;
	float m_currentFrameAction;
	float m_angularSpeed;

	AI* m_ai;
	
	AnimMode m_animMode;
	NS2::TileSheet m_idleSheet;
	NS2::TileSheet m_moveSheet;
	NS2::TileSheet m_attackSheet;
};

