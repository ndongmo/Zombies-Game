#pragma once

#include "Object.h"
#include "Gun.h"
#include "Ammo.h"

#include <glm\glm.hpp>

class Bag : public Object
{
public:
	Bag();

	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::Sound& effect, glm::vec2& direction);
	virtual void init(float x, float y, float w, float h, NS2::GLTexture& texture, glm::ivec2& texDims,
		NS2::ColorRGBA8& color, glm::vec2& direction, std::string name, float animSpeed,
		NS2::Sound& effect, int columns, int rows);

	virtual void draw(NS2::SpriteBatch& spriteBatch) override;
	virtual void draw(NS2::DebugRenderer& debugRender, NS2::ColorRGBA8& color) override;
	virtual bool use(Player* player, Level& level)override;
	virtual void textValue(std::string& str)override{ str = std::to_string(m_columns * m_rows); }
	void replace(Bag* bag);

	void setColumns(int columns){ m_columns = columns; }
	void setRows(int rows){ m_rows = rows; }

	int getColumns(){ return m_columns; }
	int getRows(){ return m_rows; }
	bool findByName(std::string& objName);
	Object* getObject(std::string& objName);
	Object* getObject(int col, int row);
	Object* getNextConsumable(Object* current);
	Gun* getNextWeapon(Gun* current);
	Ammo* getGunAmmo(Gun* current);
	void removeObject(Object* obj);
	bool addObject(Object* obj);
	bool isFull();

private:
	int m_columns;
	int m_rows;
	std::vector<Object*> m_objects;
};

