#pragma once

#include "Object.h"

class Medicine : public Object
{
public:
	Medicine();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::Sound& effect);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, float health);

	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color) override;
	virtual bool use(Player* player, Level& level)override;
	virtual void textValue(std::string& str)override{ str = std::to_string((int)m_health); }

	/* SETTERS */
	void setHealth(float health){ m_health = health; }

	/* GETTERS */
	float getHealth(){ return m_health; }

protected:
	float m_health;
};

