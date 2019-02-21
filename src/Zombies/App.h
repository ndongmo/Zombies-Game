#pragma once

#include <memory>
#include <IMainGame.h>

class GameScreen;
class MainMenuScreen;
class EditorScreen;

class App : public NS2::IMainGame
{
public:
	App();
	~App();

	void onInit() override;
	void onExit() override;
	void addScreens() override;

private:
	std::unique_ptr<GameScreen> m_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
	std::unique_ptr<EditorScreen> m_editorScreen = nullptr;
};

