#pragma once

#include "AI.h"

#include <EventHandler.h>
#include <Camera2D.h>

const float SCALE_SPEED = 0.01f;

class Player;

class PlayerAI : public AI
{
public:
	PlayerAI();
	~PlayerAI();

	virtual void init(Player* player, NS2::EventHandler* input, NS2::Camera2D* camera);
	virtual void update(Level& level, float deltaTime) override;

private:
	Player* m_player;
	NS2::Camera2D* m_camera;
	NS2::EventHandler* m_input;
};

