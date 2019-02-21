/*!
* \file Camera2D.h
* \brief Simple 2D camera.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NS2 {

	class Camera2D
	{
	public:
		/*!
		* \brief Initialize camera with width and height of screen.
		* \param screenWidth : width of screen
		* \param screenHeight : height of screen
		*/
		void init(float screenWidth, float screenHeight);
		/*!
		* \brief Update camera parameters.
		*/
		void update();
		/*!
		* \brief Convert screen coordinates to world coordinates.
		* \param screenCoords : screen coordinates
		* \return 2D vector coordinate of world
		*/
		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);
		/*!
		* \brief  Simple AABB test to see if a box is in the camera view.
		* \param position: box position
		* \param dimension : box dimension
		* \return true if box is in the camera view, otherwise return false
		*/
		bool isBoxInView(const glm::vec4& destRect);
		bool isBoxInView(const glm::vec2& position);
		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimension);
		/*!
		* \brief Increment camera position with an offset value.
		* \param offset : offset 2D vector
		*/
		void offsetPosition(const glm::vec2& offset) { m_position += offset; m_needsMatrixUpdate = true; }
		/*!
		* \brief Increment camera scale with an offset value.
		* \param offset : offset 2D vector
		*/
		void offsetScale(float offset) { m_scale += offset; if (m_scale < 0.001f) m_scale = 0.001f; m_needsMatrixUpdate = true; }

		/* SETTERS */
		void setScreenBox(glm::vec4& box);
		void setScale(float scale) { m_scale = scale; m_needsMatrixUpdate = true; }
		void setPosition(glm::vec2 position){ m_position = position; m_needsMatrixUpdate= true; }

		/* GETTERS */
		double getScreenLenght() const;
		float getScale() const				{ return m_scale; }
		float getScreenWidth() const		{ return m_screenWidth; }
		float getScreenHeight() const		{ return m_screenHeight; }
		glm::vec2 getPosition() const		{ return m_position; }
		glm::mat4 getCameraMatrix() const	{ return m_cameraMatrix; }
		float getAspectRatio() const		{ return m_screenWidth / m_screenHeight; }

	private:
		float m_screenWidth = 100.0f;					/* Screen width */
		float m_screenHeight = 100.0f;					/* Screen height */
		bool m_needsMatrixUpdate = true;				/* Keep camera update state */
		float m_scale = 1.0f;							/* Camera scale */
		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);	/* Camera position */
		glm::mat4 m_cameraMatrix = glm::mat4(1.0f);		/* Camera orthographic matrix */
		glm::mat4 m_orthoMatrix = glm::mat4(1.0f);		/* Orthographic matrix */
	};
}