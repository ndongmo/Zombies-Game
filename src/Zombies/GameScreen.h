#pragma once

#include "Level.h"
#include "App.h"
#include "GameRenderer.h"
#include "EntityController.h"
#include "LevelGenerator.h"
#include "GuiPauseMenu.h"

#include <Camera2D.h>
#include <CEGUI\CEGUI.h>
#include <IGameScreen.h>
#include <GLSLProgram.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Particle2DEngine.h>
#include <ParticleBatch2D.h>

class Zombie;

enum class GameState { PLAY, EXIT };

const float DESIRED_FPS = 60.0f;
const float MS_PER_SECOND = 1000.0f;
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
const float MAX_DELTA_TIME = 1.0f;
const int MAX_PHYSICS_STEPS = 6;

const float PLAYER_SPEED = 3.0f;
const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.0f;

class GameScreen : public NS2::IGameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

	virtual int getNextSCreenIndex() const override;
	virtual int getPreviousSCreenIndex() const override;

	Player& getPlayer(){ return m_currentLevel.player; }
	Level& getLevel(){ return m_currentLevel; }
	void setPause(bool pause);
	bool onExitCliked(const CEGUI::EventArgs& e);

private:
	void checkInput();

	/// Initializes the core system
	void initSystems();

	/// Initializes current level
	void initLevel();

	NS2::Camera2D m_camera;
	NS2::Particle2DEngine m_particleEngine;

	Level m_currentLevel;
	EntityController m_entityController;
	GameRenderer m_renderer;

	bool m_inPause;
	int m_screenWidth; ///< Screen width
	int m_screenHeight; ///< Screen height
	float m_previousTiks;
	GameState m_gameState; ///< State of the game
	GuiPauseMenu m_pauseMenu;
};

