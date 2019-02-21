#pragma once

#include "Object.h"

#include <SpriteBatch.h>
#include <DebugRenderer.h>
#include <GLTexture.h>

#define TILESIZE 40.0f

class Tile
{
public:
	Tile();
	void init(const NS2::GLTexture& texture, int x, int y, bool crossable);
	void init(Tile& tile, int x, int y);
	void reset();
	virtual void draw(NS2::SpriteBatch& spriteBatch);
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color);
	virtual bool isCrossable() const { return m_crossable; }

	/* SETTERS */
	void setColor(NS2::ColorRGBA8& color){ m_color = color; }
	void setCrossable(bool crossable){ m_crossable = crossable; }
	void setTexture(NS2::GLTexture& texture){ m_texture = texture; }
	void setPosition(int x, int y);
	void updatePosition(glm::vec2& pos){ m_destRect.x = pos.x; m_destRect.y = pos.y; }
	void setCenter(glm::vec2& center){ center.x = m_destRect.x + 
		(TILESIZE / 2.0f); center.y = m_destRect.y + (TILESIZE / 2.0f); }

	/* GETTERS */
	int getX(){ return m_position.x; }
	int getY(){ return m_position.y; }
	glm::vec2 getCenter(){ return glm::vec2(m_destRect.x + (TILESIZE / 2.0f), 
		m_destRect.y + (TILESIZE / 2.0f)); }
	const glm::ivec2& getPosition() const { return m_position; }
	const glm::vec4& getDestRect() const { return m_destRect; }
	NS2::ColorRGBA8& getColor() { return m_color; }
	NS2::GLTexture& getTexture() { return m_texture; }

protected:
	bool m_crossable;
	glm::vec4 m_destRect;
	glm::ivec2 m_position;
	static const glm::vec4 m_uvRect;
	NS2::ColorRGBA8 m_color;
	NS2::GLTexture m_texture;
};

