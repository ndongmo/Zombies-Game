#pragma once

#include "Entity.h"

#include <AudioEngine.h>

enum class ObjectType { DOOR, GUN, COFFER, MEDICINE, KEY, BAG, FLASHLIGHT, AMMO};

class Agent;

class Object : public Entity
{
public:
	Object();

	virtual void init(float x, float y, float w, float h, ShapeType type, NS2::GLTexture& texture, 
		glm::ivec2& texDims, NS2::Sound& effect);
	virtual void init(float x, float y, float w, float h, ShapeType type, NS2::GLTexture& texture,
		glm::ivec2& texDims, NS2::ColorRGBA8& color,
		glm::vec2& direction, std::string name, float animSpeed, NS2::Sound& effect, bool pickable);

	friend std::ostream& operator<<(std::ostream& os, const Object& o);

	virtual void collide(Level& level, Entity* entity, glm::vec2& collisionDepth) override;
	virtual void collide(Entity* entity){}
	virtual bool use(Player* player, Level& level) { return false; }
	virtual void applyDamage(Level& level, float damage) override {}
	virtual bool isCrossable() override { return m_pickable; }
	virtual void textAction(Player* player, std::string& str)override;
	virtual bool makeAction(Player* player, Level& level)override;
	virtual bool canInteract() override{ return true; }
	virtual bool isMergeable(){ return false; }
	virtual bool canBeMerged(Object* obj){ return false; }
	virtual void merge(Object* obj){}
	virtual void textValue(std::string& str){}

	bool isPickable(){ return m_pickable; }
	bool isConsumable(){ return m_consumable; }
	virtual void playEffect(int distance = 255);

	/* SETTERS */
	void setPickable(bool pickable){ m_pickable = pickable; }
	void setEffect(NS2::Sound effect){ m_effect = effect; }
	void setSound(NS2::Sound& sound){ m_effect = sound; }

	/* GETTERS */
	ObjectType getObjectType() { return m_objectType; }
	NS2::Sound& getSound(){ return m_effect; }

protected:
	bool m_pickable;
	bool m_consumable;
	ObjectType m_objectType;
	NS2::Sound m_effect;
};

