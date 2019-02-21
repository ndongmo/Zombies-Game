#pragma once

#include "Human.h"
#include "Light.h"

class Player : public Human
{
	friend class PlayerAI;

public:
	Player();
	virtual ~Player();

	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai);
	virtual void init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health);

	friend std::ostream& operator<<(std::ostream& os, const Player& p);

	virtual void update(Level& level, float deltaTime)override;
	virtual void draw(NS2::SpriteBatch& spriteBatch);
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color);
	virtual void collide(Level& level, Entity* entity, glm::vec2& collisionDepth) override;
	virtual void gotoIdleMode()override;
	virtual void gotoMoveMode()override;
	virtual void gotoAttackMode()override;

	bool canRun();
	float run(float deltatime);
	void equip(Object* obj);
	void equipWeapon(Gun* gun);
	void equipAmmo(Ammo* ammo);
	void changeWeapon();
	void changeObject();
	bool useObject(Level& level);
	void updateLight(Level& level, float deltatime);
	void action(Level& level);
	bool canAction();
	bool canAttack();
	void attack(Level& level);
	bool addObject(Object* obj);
	void removeObject(Object* obj);
	void gotoReloadMode();
	void reload();

	/* SETTERS */
	void setSourceLight(Light* src){ m_srcLight = src; }
	void setDestLight(Light* dest){ m_destLight = dest; }
	void setDiffLight(Light* diff){ m_diffLight = diff; }
	void setRunning(bool isRunning){ if(!m_penality) m_isRunning = isRunning; }
	void decrCurrentStamina(float value);

	/* GETTERS */
	Bag& getBag(){ return m_bag; }
	Entity* getInterAction(){ return m_interaction; }
	float getStamina(){ return m_stamina; }
	float getCurrentStamina(){ return m_currentStamina; }
	bool getPenality(){ return m_penality; }

protected:
	bool m_inAction, m_isRunning, m_penality;
	Entity* m_interaction;
	Light* m_srcLight;
	Light* m_destLight;
	Light* m_diffLight;
	float m_stamina;				/* Stamina */
	float m_speedRunStamina;		/* Losing stamina speed for running */
	float m_speedRegStamina;		/* Regaining stamina speed */
	float m_speedRun;				/* Run speed */
	float m_currentStamina;			/* Current stamina */
	float m_staminaAttack;
	float m_stepLapse;

	std::vector<std::vector<NS2::TileSheet>> m_weaponSheets;
};

