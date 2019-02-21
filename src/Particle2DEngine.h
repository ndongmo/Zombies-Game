/*!
* \file Particle2DEngine.h
* \brief Simple particle 2D engine.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <vector>

namespace NS2
{
	class IParticleBatch2D;
	class SpriteBatch;
	class Camera2D;

	class Particle2DEngine
	{
	public:
		Particle2DEngine(void);
		~Particle2DEngine(void);

		/*!
		* \brief after adding a particle batch, the particleEngine becomes responible for deallocation.
		*/
		void addParticleBatch(IParticleBatch2D* particleBatch);
		/*!
		* \brief Update all particles.
		*/
		void update(float deltaTime);
		/*!
		* \brief Draw all particles.
		*/
		void draw(SpriteBatch& spriteBatch);

	private:
		std::vector<IParticleBatch2D*> m_particleBatches; /* Array of particle batch */
	};
}
