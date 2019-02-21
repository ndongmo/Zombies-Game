#pragma once

#include "Object.h"

class Coffer : public Object
{
public:
	Coffer();
	virtual ~Coffer();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::Sound& effect, NS2::Sound& shootEffet);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed,
		NS2::Sound& effect, NS2::Sound& shootEffet, float health);

	bool isOpened(){ return m_texIndex; }
	virtual bool canInteract() override{ return !m_destructible && m_texIndex == 0; }
	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)override;
	virtual void textAction(Player* player, std::string& str)override;
	virtual bool makeAction(Player* player, Level& level)override;
	virtual void applyDamage(Level& level, float damage) override;
	virtual void destroy(Level& level) override;
	virtual void add(Object* ent);

	/* SETTERS */
	void setDestructible(bool dest){ m_destructible = dest; }
	void setHealth(float health){ m_health = health; }
	void setShootSound(NS2::Sound& sound){ m_shootEffect = sound; }

	/* GETTERS */
	bool isDestructible(){ return m_destructible; }
	float getHealth(){ return m_health; }
	NS2::Sound& getShootSound(){ return m_shootEffect; }
	std::vector<Object*>& getEntities(){ return m_entities; }

protected:
	float m_health;
	bool m_destructible;
	int m_texIndex;
	NS2::Sound m_shootEffect;
	std::vector<Object*> m_entities;
};