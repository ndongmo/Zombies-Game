#pragma once

#include "Object.h"

#include <glm\glm.hpp>
#include <string>

class Player;

class Door : public Object
{
public:
	Door();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims, 
		NS2::Sound& effect, glm::vec2& direction);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed, 
		NS2::Sound& effect, bool opened);

	virtual bool canInteract() override{ return !m_opened; }
	virtual bool isCrossable() override { return m_opened; }
	bool isOpened(){ return m_opened; }
	void setOpened(bool opened){ m_opened = opened; m_textureIndex = (opened) ? 0 : 1; }
	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)override;
	virtual void textAction(Player* player, std::string& str)override;
	virtual bool makeAction(Player* player, Level& level)override;

	void setKeyName(std::string name){ m_keyName = name; }
	std::string& getKeyName(){ return m_keyName; }

private:
	unsigned int m_textureIndex;
	std::string m_keyName;
	bool m_opened;
};

