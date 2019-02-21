#include "Bag.h"
#include "Level.h"

#define OPEN_DIFF 0.8f

Bag::Bag() : Object()
{
	m_pickable = true;
	m_consumable = true;
	m_objectType = ObjectType::BAG;
	m_name = "Bag";
	m_columns = 6;
	m_rows = 1;
	m_objects.resize(m_rows * m_columns);
}

void Bag::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::Sound& effect, glm::vec2& direction)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, effect);
	m_direction = direction;
}

void Bag::init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
	NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, 
	int columns, int rows)
{
	Object::init(x, y, w, h, ShapeType::BOX, texture, texDims, color, direction, name, animSpeed, effect, false);
	m_columns = columns;
	m_rows = rows;
	m_objects.resize(m_rows * m_columns);
}

void Bag::draw(NS2::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(m_shape.destRect, m_tileSheet.getUV(0), m_tileSheet.texture.id, 0.0f, m_color, m_direction);
}

void Bag::draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color)
{
	Object::draw(debugRender, color);
}

void Bag::replace(Bag* bag)
{
	int cols = bag->m_columns;
	int rows = bag->m_rows;
	std::string name = bag->m_name;

	bag->m_columns = this->m_columns;
	bag->m_rows = this->m_rows;
	bag->m_name = this->m_name;

	this->m_columns = cols;
	this->m_rows = rows;
	this->m_name = name;
	this->m_objects.resize(cols * rows);
}
bool Bag::use(Player* player, Level& level)
{
	player->getBag().replace(this);
	m_alive = false;
	return true;
}

bool Bag::findByName(std::string& objName)
{
	for (Object* obj : m_objects)
		if (obj != nullptr && obj->getName() == objName)
			return true;

	return false;
}
Object* Bag::getObject(std::string& objName)
{
	for (Object* obj : m_objects)
		if (obj != nullptr && obj->getName() == objName)
			return obj;

	return nullptr;
}
Object* Bag::getObject(int col, int row)
{
	if (col < m_columns && row < m_rows)
		return m_objects[(m_columns * row) + col];
	return nullptr;
}

Object* Bag::getNextConsumable(Object* current)
{
	size_t lastIndex = m_objects.size();
	
	if (current != nullptr)
	{
		bool find = false;
		for (size_t i = 0; i < m_objects.size(); i++) {
			if (m_objects[i] != nullptr) {
				if (find) {
					if (m_objects[i]->isConsumable())
						return m_objects[i];
				}
				else {
					if (m_objects[i] == current) {
						find = true;
						lastIndex = i;
					}
				}
			}
		}
	}

	for (size_t i = 0; i < lastIndex; i++) {
		if (m_objects[i] != nullptr && m_objects[i]->isConsumable())
			return m_objects[i];
	}
	return current;
}
Gun* Bag::getNextWeapon(Gun* current)
{
	size_t lastIndex = m_objects.size();

	if (current != nullptr)
	{
		bool find = false;
		for (size_t i = 0; i < m_objects.size(); i++) {
			if (m_objects[i] != nullptr) {
				if (find) {
					if (m_objects[i]->getObjectType() == ObjectType::GUN)
						return static_cast<Gun*>(m_objects[i]);
				}
				else {
					if (m_objects[i] == current) {
						find = true;
						lastIndex = i;
					}
				}
			}
		}
	}

	for (size_t i = 0; i < lastIndex; i++) {
		if (m_objects[i] != nullptr && m_objects[i]->getObjectType() == ObjectType::GUN)
			return static_cast<Gun*>(m_objects[i]);
	}
	return current;
}

Ammo* Bag::getGunAmmo(Gun* current)
{
	for (size_t i = 0; i < m_objects.size(); i++) {
		if (m_objects[i] != nullptr && m_objects[i]->getObjectType() == ObjectType::AMMO)
		{
			Ammo* ammo = static_cast<Ammo*>(m_objects[i]);
			if (ammo->getGunType() == current->getGunType())
				return ammo;
		}
	}
	return nullptr;
}

void Bag::removeObject(Object* obj)
{
	for (size_t i = 0; i < m_objects.size(); i++) {
		if (m_objects[i] != nullptr && m_objects[i] == obj) {
			m_objects[i] = nullptr;
			obj->setAlive(false);
			return;
		}
	}
}
bool Bag::addObject(Object* obj)
{
	if (obj != nullptr)
	{
		if (obj->isMergeable())
		{
			int flag = -1;

			for (size_t i = 0; i < m_objects.size(); i++) {
				if (m_objects[i] != nullptr && m_objects[i]->getObjectType() == obj->getObjectType()) {
					if (m_objects[i]->canBeMerged(obj)) {
						m_objects[i]->merge(obj);
						obj->setAlive(false);
						return true;
					}
				}
				else if (m_objects[i] == nullptr && flag < 0) {
					flag = i;
				}
			}
			if (flag >= 0) {
				m_objects[flag] = obj;
				return true;
			}
		}
		else {
			for (size_t i = 0; i < m_objects.size(); i++) {
				if (m_objects[i] == nullptr) {
					m_objects[i] = obj;
					return true;
				}
			}
		}
	}
	return false;
}

bool Bag::isFull()
{
	for (size_t i = 0; i < m_objects.size(); i++) {
		if (m_objects[i] == nullptr)
			return false;
	}
	return true;
}