#pragma once

#include <glm\glm.hpp>
#include <Vertex.h>
#include <SpriteBatch.h>
#include <GLTexture.h>
#include <AudioEngine.h>

#include <ctime>
#include <random>

enum LightType { SPOT_LIGHT, POINT_LIGHT };

class Light
{
public:
	void init(float x, float y, float radius, unsigned int index, NS2::ColorRGBA8& color, 
		LightType type = LightType::POINT_LIGHT, bool on = true, bool brokenDown = false, int percent = 100)
	{
		this->type = type;
		this->index = index;

		if (type == LightType::POINT_LIGHT) {
			uvRect = glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f);
		}
		else {
			uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		destRect.x = x;
		destRect.y = y;
		destRect.z = radius;
		destRect.w = radius;
		this->percent = percent;
		this->on = on;
		this->brokenDown = brokenDown;
		this->color = color;
	}

	void init(float x, float y, float radius, unsigned int index, NS2::ColorRGBA8& color, 
		LightType type, NS2::GLTexture& texture, bool on = true, bool brokenDown = false, int percent = 100)
	{
		init(x, y, radius, index, color, type, on, brokenDown, percent);
		this->texture = texture;
	}

	void setType(LightType type)
	{
		this->type = type;
		if (type == LightType::POINT_LIGHT)
		{
			uvRect = glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f);
			destRect.x = destRect.x;
			destRect.y = destRect.y;
		}
		else
		{
			uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			destRect.x = destRect.x;
			destRect.y = destRect.y;
		}
	}

	void draw(NS2::SpriteBatch& spriteBatch) 
	{
		if (on)
		{
			if (brokenDown)
			{
				static std::mt19937 randomEngine(time(nullptr));
				std::uniform_int_distribution<int> randOn(0, 100);
				if (percent > randOn(randomEngine))
					myDraw(spriteBatch);
			}
			else
				myDraw(spriteBatch);
		}
	}

	void myDraw(NS2::SpriteBatch& spriteBatch)
	{
		if (type == LightType::POINT_LIGHT)
		{
			spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color, 0.0f);
		}
		else
		{
			const glm::vec2 right(1.0f, 0.0f);
			float angle = acos(glm::dot(right, direction));
			if (direction.y < 0.0f)
				angle = -angle;
			angle -= 0.785f;
			spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color, angle);
		}
	}

	unsigned int index;
	unsigned int percent;
	bool on, brokenDown;
	LightType type;
	glm::vec2 direction;
	NS2::ColorRGBA8 color;
	NS2::GLTexture texture;
	glm::vec4 destRect;
	glm::vec4 uvRect;
	NS2::Sound effect;
};

