#pragma once

#include <glm\glm.hpp>
#include <ostream>
#include <DebugRenderer.h>

enum  class ShapeType
{
	CIRCLE, BOX
};

class Shape
{
public:
	Shape(float x, float y, float w, float h, ShapeType type);
	void init(float x, float y, float w, float h, ShapeType type);

	friend std::ostream& operator<<(std::ostream& os, const Shape& s);

	static bool simpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2, glm::vec2& col);
	static bool simpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2);
	static bool pointIntriangle(glm::vec2& pt, glm::vec2& t1, glm::vec2& t2, glm::vec2& t3);
	static bool inSameSide(glm::vec2& pt1, glm::vec2& t1, glm::vec2& t2);
	static bool circleContains(float x, float y, float r, glm::vec2& pt);
	static bool boxContains(float x, float y, float w, float h, glm::vec2& pt);

	bool collideWith(Shape& s, glm::vec2& collisionDepth);
	bool circleCollideCircle(Shape& c, glm::vec2& collisionDepth);
	bool circleCollideBox(Shape& c, glm::vec2& collisionDepth);
	bool collideTile(glm::vec2& centertilePos, float width);
	bool contains(glm::vec2& p);
	void draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8 color);

	/* SETTERS */
	void setPosition(const glm::vec2& pos){ position.x = destRect.x = pos.x; position.y = destRect.y = pos.y; }
	void setPosition(float x, float y){ position.x = destRect.x = x; position.y = destRect.y = y; }
	void setDestRect(glm::vec4& dest){ destRect = dest; position.x = dest.x; position.y = dest.y; }
	void incrPosition(glm::vec2& incr);
	void incrPosition(float x, float y);
	void incrPositionX(float x);
	void incrPositionY(float y);

	/* GETTERS */
	static ShapeType getTypeByName(const std::string& type);
	std::string getTypeName();
	glm::vec2 getDimension(){ return glm::vec2(destRect.z, destRect.w); }
	glm::vec2 getCenter(){ return glm::vec2(getCenterX(), getCenterY()); }
	glm::vec2 getBL(){ return glm::vec2(destRect.x, destRect.y); }
	glm::vec2 getTL(){ return glm::vec2(destRect.x, destRect.y + destRect.w); }
	glm::vec2 getTR(){ return glm::vec2(destRect.x + destRect.z, destRect.y + destRect.w); }
	glm::vec2 getBR(){ return glm::vec2(destRect.x + destRect.z, destRect.y); }
	float getWidth() { return destRect.z; }
	float getHeight() { return destRect.w; }
	float getCenterWidth() { return (destRect.z / 2.0f); }
	float getCenterHeight() { return (destRect.w / 2.0f); }
	float getCenterX() { return destRect.x + (destRect.z / 2.0f); }
	float getCenterY() { return destRect.y + (destRect.w / 2.0f); }
	float getLX(){ return destRect.x; }
	float getRX(){ return destRect.x + destRect.z; }
	float getBY(){ return destRect.y; }
	float getTY(){ return destRect.y + destRect.w; }

	ShapeType type;
	glm::vec2 position;		/* Position in earth coordinates */
	glm::vec4 destRect;
};
