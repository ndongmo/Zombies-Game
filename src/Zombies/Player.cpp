#include "Player.h"
#include "Level.h"
#include "Flashlight.h"

#include <ResourceManager.h>

#define STEP_INT 50.0f

Player::Player() : Human()
{
	m_inAction = false;
	m_isRunning = false;
	m_penality = false;
	m_name = "Player";
	m_srcLight = nullptr;
	m_destLight = nullptr;
	m_diffLight = nullptr;
	m_interaction = nullptr;
	m_stamina = 200.0f;
	m_speedRunStamina = 0.18f;
	m_speedRegStamina = 0.15f;
	m_speedRun = 2.8f;
	m_currentStamina = m_stamina;
	m_staminaAttack = 10.0f;
	m_stepLapse = 0.0f;

	m_idleSheet.init(NS2::ResourceManager::getTexture("Assets/player/fist/idle_fist.png"), glm::ivec2(5, 2));
	m_moveSheet.init(NS2::ResourceManager::getTexture("Assets/player/fist/move_fist.png"), glm::ivec2(5, 4));
	m_attackSheet.init(NS2::ResourceManager::getTexture("Assets/player/fist/meleeattack_fist.png"), glm::ivec2(5, 2));

	m_weaponSheets.resize(3);

	m_weaponSheets[GunType::HANDGUN].resize(4);
	m_weaponSheets[GunType::HANDGUN][AnimMode::ANIM_IDLE].init(
		NS2::ResourceManager::getTexture("Assets/player/handgun/idle_handgun.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::HANDGUN][AnimMode::ANIM_MOVE].init(
		NS2::ResourceManager::getTexture("Assets/player/handgun/move_handgun.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::HANDGUN][AnimMode::ANIM_RELOAD].init(
		NS2::ResourceManager::getTexture("Assets/player/handgun/reload_handgun.png"), glm::ivec2(5, 3));
	m_weaponSheets[GunType::HANDGUN][AnimMode::ANIM_ATTACK].init(
		NS2::ResourceManager::getTexture("Assets/player/handgun/shoot_handgun.png"), glm::ivec2(3, 1));

	m_weaponSheets[GunType::RIFLE].resize(4);
	m_weaponSheets[GunType::RIFLE][AnimMode::ANIM_IDLE].init(
		NS2::ResourceManager::getTexture("Assets/player/rifle/idle_rifle.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::RIFLE][AnimMode::ANIM_MOVE].init(
		NS2::ResourceManager::getTexture("Assets/player/rifle/move_rifle.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::RIFLE][AnimMode::ANIM_RELOAD].init(
		NS2::ResourceManager::getTexture("Assets/player/rifle/reload_rifle.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::RIFLE][AnimMode::ANIM_ATTACK].init(
		NS2::ResourceManager::getTexture("Assets/player/rifle/shoot_rifle.png"), glm::ivec2(3, 1));

	m_weaponSheets[GunType::SHOTGUN].resize(4);
	m_weaponSheets[GunType::SHOTGUN][AnimMode::ANIM_IDLE].init(
		NS2::ResourceManager::getTexture("Assets/player/shotgun/idle_shotgun.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::SHOTGUN][AnimMode::ANIM_MOVE].init(
		NS2::ResourceManager::getTexture("Assets/player/shotgun/move_shotgun.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::SHOTGUN][AnimMode::ANIM_RELOAD].init(
		NS2::ResourceManager::getTexture("Assets/player/shotgun/reload_shotgun.png"), glm::ivec2(5, 4));
	m_weaponSheets[GunType::SHOTGUN][AnimMode::ANIM_ATTACK].init(
		NS2::ResourceManager::getTexture("Assets/player/shotgun/shoot_shotgun.png"), glm::ivec2(3, 1));
}

void Player::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, AI* ai)
{
	Human::init(x, y, radius, texture, texDims, ai);
}

void Player::init(float x, float y, float radius, NS2::GLTexture& texture, glm::ivec2& texDims, NS2::ColorRGBA8& color,
	glm::vec2& direction, std::string name, float animSpeed, AI* ai, float speed, float health)
{
	Human::init(x, y, radius, texture, texDims, color, direction, name, animSpeed, ai, speed, health);
}

Player::~Player()
{
	m_weapon = nullptr;
}

std::ostream& operator<<(std::ostream& os, const Player& p)
{
	return os << static_cast<const Human&>(p)
		<< "gun2[" << *p.m_weapon << "])";
}

void Player::update(Level& level, float deltaTime)
{
	Human::update(level, deltaTime);
	updateLight(level, deltaTime);
	m_stepLapse += deltaTime;
	if (m_interaction != nullptr)
	{
		if (!m_interaction->getShape().collideWith(m_shape, glm::vec2())) {
			m_interaction = nullptr;
			m_inAction = false;
		}
		if (m_inAction) {
			m_currentFrameAction += deltaTime;
			if (m_currentFrameAction >= m_frameAction) {
				m_inAction = false;
				m_interaction = nullptr;
			}
		}
	}
	if (!m_inAction && !m_isRunning && m_currentStamina < m_stamina) {
		m_currentStamina += m_speedRegStamina * deltaTime;
		if (m_currentStamina >= m_stamina) {
			m_currentStamina = m_stamina;
			m_penality = false;
		}
	}

}

void Player::draw(NS2::SpriteBatch& spriteBatch)
{
	if ((m_animMode == AnimMode::ANIM_RELOAD || m_animMode == AnimMode::ANIM_ATTACK) && m_weapon != nullptr)
	{
		if (m_tileSheet.dims.x + m_tileSheet.dims.y > 2)
		{
			if (m_animMode == AnimMode::ANIM_RELOAD) {
				m_animTime += m_weapon->getReloadSpeed();
				if (m_animTime >= m_tileSheet.dims.x * m_tileSheet.dims.y)
				{
					m_animTime = 0.0f;
					int loader = m_weapon->getLoader() - m_weapon->getCurrentLoader();

					if (m_ammo->getAmount() < loader)
						loader = m_ammo->getAmount();

					m_ammo->decreaseAmount(loader);
					m_weapon->setCurrentLoader(m_weapon->getCurrentLoader() + loader);
					gotoIdleMode();
					m_inAction = false;
				}
			}
			else {
				m_animTime += m_weapon->getRate();
				if (m_animTime >= m_tileSheet.dims.x * m_tileSheet.dims.y)
				{
					m_animTime = 0.0f;
					gotoIdleMode();
					m_inAction = false;
				}
			}
		}
		if (m_invCurrentTime > 0.0f){
			spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(m_animTime), 
				m_tileSheet.texture.id, 0.0f, m_invColor, m_direction);
		}
		else
		{
			spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(m_animTime), 
				m_tileSheet.texture.id, 0.0f, m_color, m_direction);
		}
	}
	else
		Human::draw(spriteBatch);
}

void Player::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	Human::draw(debugRender, color);
}

void Player::updateLight(Level& level, float deltatime)
{
	if (m_diffLight != nullptr && m_srcLight != nullptr && m_destLight != nullptr)
	{
		if (m_object != nullptr && m_object->getObjectType() == ObjectType::FLASHLIGHT)
		{
			Flashlight* light = static_cast<Flashlight*>(m_object);
			if (light->isOn() && light->getDuration() > 0.0f)
			{
				m_diffLight->on = true;
				m_srcLight->on = true;
				m_destLight->on = true;
				m_diffLight->brokenDown = false;
				m_srcLight->brokenDown = false;
				m_destLight->brokenDown = false;

				if (light->isLow()) {
					float percent = 100 * light->getDuration() / light->getLow();
					m_diffLight->percent = percent;
					m_srcLight->percent = percent;
					m_destLight->percent = percent;
					m_diffLight->brokenDown = true;
					m_srcLight->brokenDown = true;
					m_destLight->brokenDown = true;

				}

				float deltaX = m_shape.getCenterX() - (m_diffLight->destRect.z / 2.0f);
				float deltaY = m_shape.getCenterY() - (m_diffLight->destRect.z / 2.0f);

				m_diffLight->destRect.x = deltaX;
				m_diffLight->destRect.y = deltaY;
				m_diffLight->direction = m_direction;

				deltaX = m_shape.getCenterX() - (m_srcLight->destRect.z / 2.0f) +
					(((m_srcLight->destRect.z / 2.0f) + 30 + m_shape.destRect.z) * m_direction.x);
				deltaY = m_shape.getCenterY() - (m_srcLight->destRect.z / 2.0f) +
					(((m_srcLight->destRect.z / 2.0f) + 30 + m_shape.destRect.z) * m_direction.y);
				//glm::vec2 pos1 = m_shape.getCenter() + m_shape.getDimension() * m_direction;

				m_srcLight->destRect.x = deltaX;
				m_srcLight->destRect.y = deltaY;
				m_srcLight->direction = m_direction;

				deltaX = m_srcLight->destRect.x;
				deltaY = m_srcLight->destRect.y;

				m_destLight->destRect.x = deltaX;
				m_destLight->destRect.y = deltaY;
				m_destLight->direction = m_direction;

				light->decrease(deltatime);
				return;
			}
		}

		m_diffLight->on = false;
		m_srcLight->on = false;
		m_destLight->on = false;
	}
}


void Player::collide(Level& level, Entity* entity, glm::vec2& collisionDepth)
{
	if (entity->getType() == EntityType::HUMAN){
		Human* h = static_cast<Human*>(entity);
		m_ai->getTouchSensor().getHumans().push_back(h);
		m_ai->getTouchSensor().notifyForHuman();
	}
	else if (entity->getType() == EntityType::ZOMBIE){
		Zombie* z = static_cast<Zombie*>(entity);
		m_ai->getTouchSensor().getZombies().push_back(z);
		m_ai->getTouchSensor().notifyForZombie();
	}
	else if (entity->getType() == EntityType::BULLET){
		Bullet* b = static_cast<Bullet*>(entity);
		m_ai->getTouchSensor().getBullets().push_back(b);
		m_ai->getTouchSensor().notifyForBullet();
	}
	else if (entity->getType() == EntityType::OBJECT){
		Object* o = static_cast<Object*>(entity);
		m_ai->getTouchSensor().getObjects().push_back(o);
		m_ai->getTouchSensor().notifyForObject();
		if (canAction() && o->canInteract()) m_interaction = o;
		if (o->isCrossable()) return;
	}

	m_shape.incrPosition(collisionDepth.x, collisionDepth.y);
}

void Player::action(Level& level)
{
	if (m_interaction != nullptr && !m_inAction) {
		if (m_interaction->makeAction(this, level)) {
			m_currentFrameAction = 0.0f;
			m_inAction = true;
		}
	}
}

bool Player::canAction()
{
	return !m_inAction && !m_penality;
}

bool Player::addObject(Object* obj)
{
	bool flag = m_bag.addObject(obj);
	if (flag) {
		if (obj->getObjectType() == ObjectType::GUN && m_weapon == nullptr)
			equipWeapon(static_cast<Gun*>(obj));
		else if (obj->getObjectType() == ObjectType::AMMO && m_ammo == nullptr)
			equipAmmo(static_cast<Ammo*>(obj));
		else if (m_object == nullptr && 
			obj->getObjectType() != ObjectType::GUN && obj->getObjectType() != ObjectType::AMMO)
			m_object = obj;
	}
	return flag;
}
void Player::removeObject(Object* obj)
{
	if (obj->getObjectType() == ObjectType::AMMO && obj == m_ammo) {
		m_ammo = nullptr;
	}
	else if (obj->getObjectType() == ObjectType::GUN && obj == m_weapon) {
		Gun* o = m_bag.getNextWeapon(static_cast<Gun*>(obj));
		if (o == m_weapon) m_weapon = nullptr;
		else m_weapon = o;
	}
	else if (obj == m_object) {
		Object* o = m_bag.getNextConsumable(m_object);
		if (o == m_object) m_object = nullptr;
		else m_object = o;
	}
	m_bag.removeObject(obj);
}
void Player::gotoIdleMode()
{
	if (m_weapon != nullptr && m_animMode != AnimMode::ANIM_IDLE)
	{
		m_tileSheet = m_weaponSheets[m_weapon->getGunType()][AnimMode::ANIM_IDLE];
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_IDLE;
	}
	else
		Human::gotoIdleMode();
}
void Player::gotoMoveMode()
{
	if (m_weapon != nullptr && m_animMode != AnimMode::ANIM_MOVE)
	{
		m_tileSheet = m_weaponSheets[m_weapon->getGunType()][AnimMode::ANIM_MOVE];
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_MOVE;
	}
	else
		Human::gotoMoveMode();
}
void Player::gotoAttackMode()
{
	if (m_weapon != nullptr && m_animMode != AnimMode::ANIM_ATTACK)
	{
		m_tileSheet = m_weaponSheets[m_weapon->getGunType()][AnimMode::ANIM_ATTACK];
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_ATTACK;
	}
	else
		Human::gotoAttackMode();
}
void Player::gotoReloadMode()
{
	if (m_animMode != AnimMode::ANIM_RELOAD)
	{
		m_tileSheet = m_weaponSheets[m_weapon->getGunType()][AnimMode::ANIM_RELOAD];
		m_animTime = 0.0f;
		m_animMode = AnimMode::ANIM_RELOAD;
		if (m_weapon->getGunType() == GunType::HANDGUN)
			NS2::ResourceManager::getSound("Sounds/handgun_reload.ogg").play();
		else if (m_weapon->getGunType() == GunType::RIFLE)
			NS2::ResourceManager::getSound("Sounds/rifle_reload.ogg").play();
		else if (m_weapon->getGunType() == GunType::SHOTGUN)
				NS2::ResourceManager::getSound("Sounds/shotgun_reload.ogg").play();
	}
}
bool Player::canAttack()
{
	return m_weapon != nullptr && m_currentStamina >= m_staminaAttack &&
		((m_ammo != nullptr && m_ammo->getAmount() > 0) || m_weapon->getCurrentLoader() > 0)
		&& canAction();
}
void Player::attack(Level& level)
{
	if (canAttack())
	{
		if (m_weapon->getCurrentLoader() <= 0)
			gotoReloadMode();
		else {
			m_weapon->fire(level, this);
			gotoAttackMode();
			decrCurrentStamina(m_staminaAttack);
		}
		m_inAction = true;
	}
}
bool Player::useObject(Level& level)
{
	if (canAction() && m_object != nullptr) {
		if (m_object->use(this, level)) {
			NS2::ResourceManager::getSound("Sounds/use.ogg").play();
			m_bag.removeObject(m_object);
			m_object = m_bag.getNextConsumable(nullptr);
			return true;
		}
	}
	return false;
}
void Player::changeWeapon()
{
	if (canAction()) {
		Gun* weapon = m_bag.getNextWeapon(m_weapon);
		if (weapon != nullptr && weapon != m_weapon)
		{
			NS2::ResourceManager::getSound("Sounds/equip.ogg").play();
			equipWeapon(weapon);
		}
	}
}
void Player::changeObject()
{
	if (canAction()) {
		NS2::ResourceManager::getSound("Sounds/change.mp3").play();
		m_object = m_bag.getNextConsumable(m_object);
	}
}

void Player::equipWeapon(Gun* gun)
{
	m_weapon = gun;
	m_ammo = m_bag.getGunAmmo(gun);
	m_tileSheet = m_weaponSheets[gun->getGunType()][AnimMode::ANIM_IDLE];
}
void Player::equipAmmo(Ammo* ammo)
{
	if (m_weapon != nullptr) {
		m_ammo = m_bag.getGunAmmo(m_weapon);
	}
}
void Player::equip(Object* obj)
{
	if (obj != nullptr) {
		NS2::ResourceManager::getSound("Sounds/equip.ogg").play();

		if (obj->getObjectType() == ObjectType::GUN)
			equipWeapon(static_cast<Gun*>(obj));
		else if (obj->getObjectType() != ObjectType::AMMO)
			m_object = obj;
	}
}

float Player::run(float deltatime)
{
	float speed = m_speed;
	if (m_isRunning) {
		decrCurrentStamina(m_speedRunStamina * deltatime);
		speed = m_speedRun;
	}

	if (m_stepLapse >= STEP_INT / speed) {
		m_stepLapse = 0.0f;
		NS2::ResourceManager::getSound("Sounds/footstep.ogg").play();
	}

	return speed;
}
void Player::decrCurrentStamina(float value)
{
 	m_currentStamina -= value;
	if (m_currentStamina <= 0.0f) {
		m_currentStamina = 0.0f;
		m_penality = true;
		m_isRunning = false;
	}
}
void Player::reload()
{
	if (m_ammo != nullptr && m_ammo->getAmount() > 0 && m_weapon != nullptr && 
		m_weapon->getCurrentLoader() < m_weapon->getLoader()
		&& canAction()) {
		gotoReloadMode();
		m_inAction = true;
	}
}