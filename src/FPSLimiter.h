/*!
* \file FPSLimiter.h
* \brief Calculate and limite game FPS.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

namespace NS2 {

#define NUM_SAMPLES 10

	class FPSLimiter {
	public:
		/*!
		* \brief Initialize with max FPS.
		* \param maxFPS: maximum FPS game can reach
		*/
		void init(float maxFPS);
		/*!
		* \brief Retrieve SDL ticks.
		*/
		void begin();
		/*!
		* \brief Calculate and return FPS value.
		*/
		void end();

		/* SETTERS */
		void setMaxFPS(float maxFPS);

		/* GETTERS */
		float getFPS() const { return m_fps; }

	private:
		/*!
		* \brief Calculate FPS value.
		*/
		void calculateFPS();

		float m_fps;				/* FPS value */
		float m_maxFPS;				/* Maximum FPS value */
		float m_frameTime;			/* Frame time */
		unsigned int m_startTicks;	/* Keep ticks last value */
	};
}