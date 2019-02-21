#pragma once

#include "Level.h"

#include <EventHandler.h>
#include <Camera2D.h>

class LevelGenerator
{
public:
	LevelGenerator();
	void init(NS2::EventHandler& input, NS2::Camera2D& camera, NS2::AudioEngine& audioEngine);

	Level* createLevel(int width, int height, int cellSize);

private:
	NS2::Camera2D* m_camera;
	NS2::EventHandler* m_input;
	NS2::AudioEngine* m_audioEngine;
};

