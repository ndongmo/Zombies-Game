#pragma once

#include "Object.h"
#include "Light.h"

#include <glm\glm.hpp>

class Flashlight : public Object
{
public:
	Flashlight();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::Sound& effect, glm::vec2& direction);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed,
		NS2::Sound& effect, int duration, int low);

	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color) override;
	virtual void update(Level& level, float deltaTime)override;
	virtual bool use(Player* player, Level& level)override;
	virtual bool isMergeable()override { return true; }
	virtual bool canBeMerged(Object* obj)override{ return true; }
	virtual void merge(Object* obj)override;
	virtual void textValue(std::string& str)override{ str = std::to_string((int)(m_duration / 100)); }

	void decrease(float deltatime){ m_duration -= deltatime; }
	bool isOn(){ return m_on && m_duration > 0.0f; }
	bool isLow(){ return m_duration < m_low; }
	void setOn(bool on){ m_on = on; }
	void setDuration(float duration){ m_duration = duration; }
	void setLow(float low){ m_low = low; }

	float getDuration() { return m_duration; }
	float getLow() { return m_low; }

private:
	float m_duration;
	float m_low;
	bool m_on;
};

