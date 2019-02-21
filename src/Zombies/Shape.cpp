#include "Shape.h"

Shape::Shape(float x, float y, float width, float height, ShapeType type) :
position(x, y), destRect(x, y, width, height), type(type)
{
}

void Shape::init(float x, float y, float w, float h, ShapeType type)
{
	destRect.x = position.x = x;
	destRect.y = position.y = y;
	destRect.z = w;
	destRect.w = h;
	type = type;
}

std::ostream& operator<<(std::ostream& os, const Shape& s)
{
	os << ((s.type == ShapeType::BOX) ?  "BOX" : "CIRCLE")
		<< "["
		<< s.destRect.x << ", " << s.destRect.y << ", "
		<< s.destRect.z << ", " << s.destRect.w 
	   << "]";

	return os;
}

std::string Shape::getTypeName()
{
	return (type == ShapeType::BOX) ? "BOX" : "CIRCLE";
}

ShapeType Shape::getTypeByName(const std::string& type)
{
	return (type == "BOX") ? ShapeType::BOX : ShapeType::CIRCLE;
}

void Shape::draw(NS2::DebugRenderer& debugRenderer, NS2::ColorRGBA8 color)
{
	if (type == ShapeType::BOX)
		debugRenderer.drawBox(destRect, color, 0.0f);
	else
		debugRenderer.drawCircle(getCenter(), color, getCenterWidth());
}

bool Shape::contains(glm::vec2& p)
{
	return (
		p.x > destRect.x && p.x < destRect.x + destRect.z &&
		p.y > destRect.y && p.y < destRect.y + destRect.w
	);
}

bool Shape::collideWith(Shape& s, glm::vec2& collisionDepth)
{
	if (s.type == ShapeType::CIRCLE && type == ShapeType::CIRCLE)
		return circleCollideCircle(s, collisionDepth);
	else
		return circleCollideBox(s, collisionDepth);
}

void Shape::incrPosition(glm::vec2& incr)
{
	incrPositionX(incr.x);
	incrPositionY(incr.y);
}

void Shape::incrPosition(float x, float y)
{
	incrPositionX(x);
	incrPositionY(y);
}

void Shape::incrPositionX(float x)
{
	position.x += x;
	destRect.x = position.x;
}

void Shape::incrPositionY(float y)
{
	position.y += y;
	destRect.y = position.y;
}

bool Shape::circleCollideCircle(Shape& s, glm::vec2& collisionDepth)
{
	float minDist = getCenterWidth() + s.getCenterWidth();
	glm::vec2 distVec = getCenter() - s.getCenter();
	float collision = minDist - glm::length(distVec);

	if (collision > 0) {
		if (collision == minDist) 
			collisionDepth = glm::normalize(glm::vec2(1.0f,1.0f)) * collision;
		else
			collisionDepth = glm::normalize(distVec) * collision;
		return true;
	}

	return false;
}

bool Shape::circleCollideBox(Shape& s, glm::vec2& collisionDepth)
{
	float minDistX = getCenterWidth() + s.getCenterWidth();
	float minDistY = getCenterHeight() + s.getCenterHeight();

	glm::vec2 distVec = getCenter() - s.getCenter();
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if (collisionX > 0 && collisionY > 0) {
		if (collisionX < collisionY) {
			collisionDepth.x = (distVec.x < 0) ? -collisionX : collisionX;
		}
		else {
			collisionDepth.y = (distVec.y < 0) ? -collisionY : collisionY;
		}
		
		return true;
	}

	return false;
}

bool Shape::collideTile(glm::vec2& centertilePos, float width)
{
	float minDistX = getCenterWidth() + (width / 2.0f);
	float minDistY = getCenterHeight() + (width / 2.0f);

	glm::vec2 distVec = getCenter() - centertilePos;
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if (collisionX > 0 && collisionY > 0) {
		if (collisionX < collisionY) {
			if (distVec.x < 0) {
				incrPositionX(-collisionX);
			}
			else {
				incrPositionX(collisionX);
			}
		}
		else {
			if (distVec.y < 0) {
				incrPositionY(-collisionY);
			}
			else {
				incrPositionY(collisionY);
			}
		}
		return true;
	}

	return false;
}

bool Shape::simpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2, glm::vec2& col)
{
	float minDistX = x1 + x2;
	float minDistY = y1 + y2;

	glm::vec2 distVec = c1 - c2;
	col.x = minDistX - abs(distVec.x);
	col.y = minDistY - abs(distVec.y);

	if (col.x > 0 && col.y > 0) {
		return true;
	}

	return false;
}

bool Shape::simpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2)
{
	float minDistX = x1 + x2;
	float minDistY = y1 + y2;

	glm::vec2 distVec = c1 - c2;
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if (collisionX > 0 && collisionY > 0) {
		return true;
	}

	return false;
}

bool Shape::pointIntriangle(glm::vec2& pt, glm::vec2& t1, glm::vec2& t2, glm::vec2& t3)
{
	bool b1 = inSameSide(pt, t1, t2);
	bool b2 = inSameSide(pt, t2, t3);
	bool b3 = inSameSide(pt, t3, t1);
	return b1 && b2 && b3;
}

bool Shape::inSameSide(glm::vec2& pt, glm::vec2& t1, glm::vec2& t2)
{
	float v = ((t2.y - t1.y)*(pt.x - t1.x) + (-t2.x + t1.x)*(pt.y - t1.y));
	return v >= 0.0f;
}

bool Shape::circleContains(float x, float y, float r, glm::vec2& pt)
{
	return sqrt(pow(x - pt.x, 2) + pow(y - pt.y, 2)) <= r;
}

bool Shape::boxContains(float x, float y, float w, float h, glm::vec2& pt)
{
	return (pt.x > x && pt.x < x + w && pt.y > y && pt.y < y + h );
}