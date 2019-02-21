/*!
* \file Object.h
* \brief Player Renderer class.
* It aims to render player attributes such as life, stamina, money and so on.
* \author Ndongmo Silatsa Fabrice
* \date 12-10-2017
* \version 2.0
*/

#pragma once

#include "Level.h"

class PlayerRenderer
{
public:
	PlayerRenderer();
	/*!
	* \brief Initialize Player menu renderer.
	* \param screenWidth : screen width
	* \param screenHeight : screen height
	*/
	void init(float screenWidth, float screenHeight);
	/*!
	* \brief renders player menu.
	* \param spriteBatch : sprite batch
	* \param level : current level
	* \param box : screen box
	*/
	void render(NS2::SpriteBatch& spriteBatch, Level& level, const glm::vec4& box);
	/*!
	* \brief Updates the vector of temporary points given the current vector and the screen box.
	* \param points : vector of points
	* \param box : screen box
	*/
	void updatePoints(const glm::vec4& points, const glm::vec4& box);
	/*!
	* \brief Updates the vector of temporary points given the current vector and the screen box.
	* \param points : vector of points
	* \param box : screen box
	* \param epsilon : width multiplicator
	*/
	void updatePoints(const glm::vec4& points, const glm::vec4& box, float epsilon);

private:
	NS2::ColorRGBA8 m_staminaLowColor;
	NS2::ColorRGBA8 m_dialogColor;
	NS2::ColorRGBA8 m_dialogFontColor;			/* Font color */
	NS2::ColorRGBA8 m_fontColor;				/* Font color */
	NS2::ColorRGBA8 m_color;					/* Renderer color */
	NS2::GLTexture m_playerBack;				/* Player image background texture */
	NS2::GLTexture m_lifeBack;					/* Player life bar background texture */
	NS2::GLTexture m_dialogBack;				/* Player dialog box background texture */
	NS2::GLTexture m_stuffBack;					/* Player stuff background texture */
	NS2::GLTexture m_moneyBack;					/* Player money background texture */
	NS2::GLTexture m_lifeFront;					/* Player life bar foreground texture */
	NS2::SpriteFont m_spriteFont;				/* Player render font */
	NS2::SpriteFont m_smallFont;				/* Player render font */
	NS2::GLTexture m_staminaBack;				/* Player stamina bar background texture */
	NS2::GLTexture m_staminaFront;				/* Player stamina bar foreground texture */
	glm::vec4 m_uvRect;							/* UV rectangle */
	glm::vec4 m_playerRect;						/* Player image positions */
	glm::vec4 m_lifeRect;						/* Player life bar positions */
	glm::vec4 m_dialogRect;						/* Player dialog box positions */
	glm::vec4 m_topStuffRect;					/* Player stuff top positions */
	glm::vec4 m_leftStuffRect;					/* Player stuff left positions */
	glm::vec4 m_rightStuffRect;					/* Player stuff right positions */
	glm::vec4 m_moneyRect;						/* Player money positions */
	glm::vec4 m_staminaRect;						/* Player stamina bar positions */
	std::vector<glm::vec2> m_tmpRect;			/* Temporary positions */
};

