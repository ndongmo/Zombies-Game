#pragma once

#include "Shape.h"

#include <string>
#include <glm\glm.hpp>
#include <SpriteBatch.h>
#include <Vertex.h>
#include <TileSheet.h>
#include <GLTexture.h>
#include <DebugRenderer.h>

enum class EntityState{DYNAMIC, STATIC};
enum class EntityType{ZOMBIE, HUMAN, OBJECT, BULLET, NONE};

template<class T>
struct Cell;
class Level;
class Player;

class Entity
{
public:
	Entity();
	virtual void init(float x, float y, float w, float h, ShapeType type, NS2::GLTexture& texture, glm::ivec2& texDims);
	virtual void init(float x, float y, float w, float h, ShapeType type, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed);

	virtual ~Entity();

	friend std::ostream& operator<<(std::ostream& os, const Entity& e);

	virtual void draw(NS2::SpriteBatch& spriteBatch);
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color);
	virtual void update(Level& level, float deltaTime){}
	virtual void destroy(Level& level);
	virtual void endInit(){}

	virtual void checkCollision(Level& level){}
	virtual void collide(Level& level, Entity* entity, glm::vec2& collisionDepth) = 0;
	virtual void applyDamage(Level& level, float damage) = 0;
	virtual bool isCrossable() { return false; }
	virtual void textAction(Player* player, std::string& str) {  }
	virtual bool makeAction(Player* player, Level& level) { return false; }
	virtual bool canInteract() { return false; }

	/* SETTERS */
	void setCell(Cell<Entity>* cell){ m_cell = cell; }
	void setCellIndex(int index){ m_cellIndex = index; }
	void setIndex(int index){ m_index = index; }
	void setName(std::string name){ m_name = name; }
	void setAnimSpeed(float animSpeed){ m_animSpeed = animSpeed;}
	void setColor(NS2::ColorRGBA8& color) { m_color = color; }
	void setTexture(NS2::GLTexture& texture, glm::ivec2& texDims){ m_tileSheet.init(texture, texDims); }
	void setDirection(glm::vec2& direction){ m_direction = direction; }
	void setAlive(bool alive){ m_alive = alive; }
	void setState(EntityState state){ m_state = state; }
	void setType(EntityType type){ m_type = type; }
	void setOnAnim(bool anim){ m_onAnim = anim; }

	/* GETTERS */
	const std::string& getName(){ return m_name; }
	Shape& getShape() { return m_shape; }
	glm::vec2& getDirection(){ return m_direction; }
	Cell<Entity>* getCell(){ return m_cell; }
	int getCellIndex(){ return m_cellIndex; }
	int getIndex(){ return m_index; }
	EntityState getState(){ return m_state; }
	EntityType getType(){ return m_type; }
	bool isAlive(){ return m_alive; }
	float getAnimSpeed(){ return m_animSpeed; }
	NS2::TileSheet& getTexture(){ return m_tileSheet; }
	NS2::ColorRGBA8& getColor(){ return m_color; }

protected:
	EntityState m_state;
	EntityType m_type;
	int m_cellIndex;
	Cell<Entity>* m_cell;

	int m_index; // Index of entity in the entities list
	float m_angle;
	float m_animSpeed;
	float m_animTime;
	bool m_alive;
	bool m_onAnim;

	Shape m_shape;
	std::string m_name;
	NS2::TileSheet m_tileSheet;
	NS2::ColorRGBA8 m_color;
	glm::vec2 m_direction;
};

