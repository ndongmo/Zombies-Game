#pragma once

#include "Agent.h"
#include "Level.h"
#include "LevelEditor.h"
#include "Entity.h"
#include "Tile.h"
#include "Light.h"
#include "GraphEdge.h"
#include "GraphNode.h"
#include "Grid.h"
#include "PlayerRenderer.h"

#include <Camera2D.h>
#include <SpriteBatch.h>
#include <GLSLProgram.h>
#include <Camera2D.h>
#include <SpriteFont.h>
#include <Particle2DEngine.h>
#include <ParticleBatch2D.h>
#include <vector>

class Renderer
{
protected:
	NS2::ColorRGBA8 m_debugColor;
};

class EntityRenderer : public Renderer
{
public:
	void init();
	void renderPalyerStuff(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, Level& level);
	void render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, std::vector<Entity*>& entities);
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Entity*>& entities);
	void render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, Level& level);
	void render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, LevelEditor& level);
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, LevelEditor& level);
private:
	float m_offset, m_bloodWidth, m_bloodHeight, m_playerBloodWidth, m_playerBloodHeight;
	bool m_showAgentBlood;
	glm::vec4 m_desRect, m_uvRect;
	NS2::ColorRGBA8 m_clBlood, m_clBloodBack, m_clBloodBackground, m_clFont;
	NS2::GLTexture m_bloodTexture, m_bloodBackTexture;
	NS2::SpriteFont m_spriteFont;
};

class TileRenderer : public Renderer
{
public:
	TileRenderer();
	void render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, std::vector<Tile>& tiles);
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Tile>& tiles);
};

class GraphRenderer : public Renderer
{
public:
	GraphRenderer();
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<GraphNode>& nodes, 
		std::vector<GraphEdge>& edges);
};

class LightRenderer : public Renderer
{
public:
	LightRenderer();
	void render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, std::vector<Light>& lights);
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Light>& lights);
};

class GridRenderer : public Renderer
{
public:
	GridRenderer();
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, Grid<Entity>& grid);
};

class AIRenderer : public Renderer
{
public:
	AIRenderer();
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Entity*>& entities);
};

class MapRenderer : public Renderer
{
public:
	MapRenderer();
	void render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, Level& level);

private:
	float m_screenRatio, m_border;
	GLubyte m_transparency;
	glm::ivec2 m_position;
	bool m_showTiles, m_showZombies, m_showHumans, m_showPlayer, m_showObjects;
	NS2::ColorRGBA8 m_clTiles, m_clZombies, m_clHumans, m_clPlayer, m_clObjects;
};


class GameRenderer
{
public:
	void init(float screenWidth, float screenHeight);
	void clearAmbientColor(const NS2::ColorRGBA8& color);
	void render(Level& level, NS2::Camera2D& camera, NS2::Particle2DEngine& particleEngine);
	void render(LevelEditor& levelEditor, NS2::Camera2D& camera, bool isMouseInMap);
	void dispose();
	void setDebug(bool debug){ m_debug = debug; }
	void setLineWidth(float lineWidth){ m_lineWidth = lineWidth; }

private:
	bool m_debug;
	float m_lineWidth;

	NS2::ColorRGBA8 m_debugColor;
	NS2::GLSLProgram m_lightProgram;
	NS2::GLSLProgram m_textureProgram; ///< The shader program
	NS2::SpriteBatch m_spriteBatch; ///< Draws all agents
	NS2::SpriteBatch m_lightSpriteBatch; ///< Draws all lights
	NS2::SpriteBatch m_fontSpriteBatch; ///< Draws fonts
	NS2::DebugRenderer m_debugRenderer;

	PlayerRenderer m_playerRenderer;
	EntityRenderer m_entityRenderer;
	TileRenderer m_tileRenderer;
	LightRenderer m_lightRenderer;
	GraphRenderer m_graphRenderer;
	GridRenderer m_gridRenderer;
	AIRenderer m_aiRenderer;
	MapRenderer m_mapRenderer;
	NS2::GLTexture m_lightMap;
};