#include "Camera2D.h"

namespace NS2 {

	void Camera2D::init(float screenWidth, float screenHeight) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, m_screenWidth, 0.0f, m_screenHeight);
	}

	void Camera2D::update() {
		if(m_needsMatrixUpdate) {
			// camera translation
			glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

			// camera scale
			glm::vec3 scale (m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

			m_needsMatrixUpdate = false;
		}
	}

	double Camera2D::getScreenLenght() const{
		return sqrt((m_screenWidth * m_screenWidth) + (m_screenHeight * m_screenHeight));
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		// invert y direction
		screenCoords.y = m_screenHeight - screenCoords.y;
		// Make it so that 0 is the center
		screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);

		// Scale the coordinates
		screenCoords /= m_scale;

		// Translate wth the camera position
		screenCoords += m_position;

		return screenCoords;
	}

	void Camera2D::setScreenBox(glm::vec4& box)
	{
		box.x = m_position.x - (m_screenWidth / (2 * m_scale));
		box.y = m_position.y - (m_screenHeight / (2 * m_scale));
		box.z = m_screenWidth;
		box.w = m_screenHeight;
	}

	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimension) {
		// Convert screen dimension in scaled dimension
		glm::vec2 scaleDimension = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;
		// The minimum distance before a collision occurs
		const float MIN_DISTANCE_X = (scaleDimension.x / 2.0f) + (dimension.x / 2.0f);
		const float MIN_DISTANCE_Y = (scaleDimension.y / 2.0f) + (dimension.y / 2.0f);

		// Center position of the parameters
		glm::vec2 centerPos = position + (dimension / 2.0f);
		// Center position of the camera
		glm::vec2 centerCameraPos = m_position;
		glm::vec2 distVec = centerCameraPos - centerPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		return (xDepth > 0 && yDepth > 0);
	}

	bool Camera2D::isBoxInView(const glm::vec2& position) {
		// Convert screen dimension in scaled dimension
		glm::vec2 scaleDimension = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;
		// The minimum distance before a collision occurs
		const float MIN_DISTANCE_X = (scaleDimension.x / 2.0f);
		const float MIN_DISTANCE_Y = (scaleDimension.y / 2.0f);

		float xDepth = MIN_DISTANCE_X - abs(m_position.x - position.x);
		float yDepth = MIN_DISTANCE_Y - abs(m_position.y - position.y);

		return (xDepth > 0 && yDepth > 0);
	}

	bool Camera2D::isBoxInView(const glm::vec4& destRect) {
		// Convert screen dimension in scaled dimension
		glm::vec2 scaleDimension = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;
		// The minimum distance before a collision occurs
		const float MIN_DISTANCE_X = (scaleDimension.x / 2.0f) + (destRect.z / 2.0f);
		const float MIN_DISTANCE_Y = (scaleDimension.y / 2.0f) + (destRect.w / 2.0f);

		float xDepth = MIN_DISTANCE_X - abs(m_position.x - (destRect.x + (destRect.z / 2.0f)));
		float yDepth = MIN_DISTANCE_Y - abs(m_position.y - (destRect.y + (destRect.w / 2.0f)));

		return (xDepth > 0 && yDepth > 0);
	}
}