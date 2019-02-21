/*!
* \file ParticleBatch2D.h
* \brief Draws batched particle 2D.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "GLTexture.h"
#include "IParticleBatch2D.h"

namespace NS2
{
	class ParticleBatch2D : public IParticleBatch2D
	{
	public:
		/*!
		* \brief Initialize particle batch stuffs.
		* \param maxParticles : number maximum of particles which can be generated
		* \param decayRate : Reduced life rate after a deltaTime
		* \param texture : particle texture
		* \param updateFunc : update particle function 
		*/
		void init(int maxParticles, float decayRate, GLTexture texture,
			std::function<void(Particle2D&, float deltaTime)> updateFunc = defaultParticleUpdate);

		virtual void draw(SpriteBatch& spriteBatch, Particle2D& particle) override;

	private:
		glm::vec4 m_uvRect;
		GLTexture m_texture;	/* particle texture */
	};
}