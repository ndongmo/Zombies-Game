#include "PlayerAI.h"
#include "EventConfig.h"
#include "Player.h"

#include <ResourceManager.h>

PlayerAI::PlayerAI() : AI()
{
	m_name = "PLAYER_AI";
}

PlayerAI::~PlayerAI()
{
	m_player = nullptr;
	m_input = nullptr;
	m_camera = nullptr;
}

void PlayerAI::init(Player* player, NS2::EventHandler* input, NS2::Camera2D* camera)
{
	m_player = player;
	m_input = input;
	m_camera = camera;
}

void PlayerAI::update(Level& level, float deltaTime)
{
	if (!m_player->canAction()) return;

	if (m_input->isEventDown(MyEvent::GO_UP)) {
		float speed = m_player->run(deltaTime);
		m_player->m_shape.incrPositionY(speed * deltaTime);
	}
	if (m_input->isEventDown(MyEvent::GO_DOWN)) {
		float speed = m_player->run(deltaTime);
		m_player->m_shape.incrPositionY(-speed * deltaTime);
	}
	if (m_input->isEventDown(MyEvent::GO_LEFT)) {
		float speed = m_player->run(deltaTime);
		m_player->m_shape.incrPositionX(-speed * deltaTime);
	}
	if (m_input->isEventDown(MyEvent::GO_RIGHT)) {
		float speed = m_player->run(deltaTime);
		m_player->m_shape.incrPositionX(speed * deltaTime);
	}
	if (m_input->isEventDown(MyEvent::ACTION)) {
		m_input->releaseEvent(MyEvent::ACTION);
		m_player->action(level);
	}
	if (m_input->isEventDown(MyEvent::ATTACK)) {
		m_input->releaseEvent(MyEvent::ATTACK);
		m_player->attack(level);
	}
	if (m_input->isEventDown(MyEvent::CHANGE_WEAPON)) {
		m_input->releaseEvent(MyEvent::CHANGE_WEAPON);
		m_player->changeWeapon();
	}
	if (m_input->isEventDown(MyEvent::CHANGE_OBJECT)) {
		m_input->releaseEvent(MyEvent::CHANGE_OBJECT);
		m_player->changeObject();
	}
	if (m_input->isEventDown(MyEvent::USE_OBJECT)) {
		m_input->releaseEvent(MyEvent::USE_OBJECT);
		m_player->useObject(level);
	}
	if (m_input->isEventDown(MyEvent::RELOAD)) {
		m_input->releaseEvent(MyEvent::RELOAD);
		m_player->reload();
	}
	if (m_input->isEventDown(MyEvent::RUN)) {
		m_player->setRunning(true);
	}
	else {
		m_player->setRunning(false);
	}

	glm::vec2 mouseCoords = m_input->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
	m_player->rotate(level, mouseCoords, deltaTime);
}
