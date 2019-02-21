#pragma once

#include "GuiEditor.h"
#include "ScreenIndices.h"
#include "LevelEditor.h"
#include "GameRenderer.h"

#include <vector>
#include <Camera2D.h>
#include <DebugRenderer.h>
#include <GLSLProgram.h>
#include <GLTexture.h>
#include <IGameScreen.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <GUI.h>


class EditorScreen : public NS2::IGameScreen {
public:

	friend class GuiEditor;

	EditorScreen();
	~EditorScreen();

	/************************************************************************/
	/* IGameScreen Implementation                                           */
	/************************************************************************/
	virtual int getNextSCreenIndex() const override;
	virtual int getPreviousSCreenIndex() const override;
	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;

	bool saveLevel(std::string& error);
	bool loadLevel(std::string& fileName, std::string& error);

private:

	void clearLevel();

	void initGui();
	void checkInput();
	void updateMouseDown(const SDL_Event& evnt);
	void updateMouseUp(const SDL_Event& evnt);
	void updateMouseWeel(const SDL_Event& evnt);
	void updateMouseMotion(const SDL_Event& evnt);

	bool isMouseInMap();
	bool isMouseInUI();
	
	void tileMouseLeftDown(glm::vec2& pos);
	void tileMouseRightDown(glm::vec2& pos);
	void tileMouseMotion(glm::vec2& pos);
	void tileMouseDragging(glm::vec2& pos);

	void lightMouseLeftDown(glm::vec2& pos);
	void lightMouseRightDown(glm::vec2& pos);
	void lightMouseMotion(glm::vec2& pos);
	void lightMouseWheel(float offset);

	void nodeMouseLeftDown(glm::vec2& pos);
	void nodeMouseRightDown(glm::vec2& pos);
	void nodeMouseMotion(glm::vec2& pos);
	void nodeMouseWheel(float offset);

	void humanMouseLeftDown(glm::vec2& pos);
	void humanMouseRightDown(glm::vec2& pos);
	void humanMouseMotion(glm::vec2& pos);
	void humanMouseWheel(float offset);

	void zombieMouseLeftDown(glm::vec2& pos);
	void zombieMouseRightDown(glm::vec2& pos);
	void zombieMouseMotion(glm::vec2& pos);
	void zombieMouseWheel(float offset);

	void playerMouseLeftDown(glm::vec2& pos);
	void playerMouseRightDown(glm::vec2& pos);
	void playerMouseMotion(glm::vec2& pos);
	void playerMouseWheel(float offset);

	void cofferMouseLeftDown(glm::vec2& pos);
	void cofferMouseRightDown(glm::vec2& pos);
	void cofferMouseMotion(glm::vec2& pos);
	void cofferMouseWheel(float offset);

	void keyMouseLeftDown(glm::vec2& pos);
	void keyMouseRightDown(glm::vec2& pos);
	void keyMouseMotion(glm::vec2& pos);
	void keyMouseWheel(float offset);

	void doorMouseLeftDown(glm::vec2& pos);
	void doorMouseRightDown(glm::vec2& pos);
	void doorMouseMotion(glm::vec2& pos);
	void doorMouseWheel(float offset);

	void gunMouseLeftDown(glm::vec2& pos);
	void gunMouseRightDown(glm::vec2& pos);
	void gunMouseMotion(glm::vec2& pos);
	void gunMouseWheel(float offset);

	void medocMouseLeftDown(glm::vec2& pos);
	void medocMouseRightDown(glm::vec2& pos);
	void medocMouseMotion(glm::vec2& pos);
	void medocMouseWheel(float offset);

	void bagMouseLeftDown(glm::vec2& pos);
	void bagMouseRightDown(glm::vec2& pos);
	void bagMouseMotion(glm::vec2& pos);
	void bagMouseWheel(float offset);

	void ammoMouseLeftDown(glm::vec2& pos);
	void ammoMouseRightDown(glm::vec2& pos);
	void ammoMouseMotion(glm::vec2& pos);
	void ammoMouseWheel(float offset);

	void flashlightMouseLeftDown(glm::vec2& pos);
	void flashlightMouseRightDown(glm::vec2& pos);
	void flashlightMouseMotion(glm::vec2& pos);
	void flashlightMouseWheel(float offset);

	/************************************************************************/
	/* Member Variables                                                     */
	/************************************************************************/
	
	bool m_mouseButtons[2];
	bool m_isDragging;
	bool m_isOnMap;
	bool m_isOnUi;

	glm::vec2 m_selectOffset;

	NS2::Camera2D m_camera;
	GuiEditor m_guiEditor;
	GameRenderer m_renderer;
	LevelEditor m_level;
};