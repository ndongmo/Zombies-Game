/*!
* \file FontParticleBatch2D.h
* \brief Draws batched font particle 2D.
* \author Ndongmo Silatsa Fabrice
* \date 24-02-2017
* \version 1.0
*/

#pragma once

#include "SpriteFont.h"
#include "IParticleBatch2D.h"

namespace NS2
{
	class FontParticleBatch2D : public IParticleBatch2D
	{
	public:
		/*!
		* \brief Initialize particle batch stuffs.
		* \param maxParticles : number maximum of particles which can be generated
		* \param decayRate : Reduced life rate after a deltaTime
		* \param texture : particle texture
		* \param updateFunc : update particle function
		*/
		void init(int maxParticles, float decayRate, SpriteFont& font,
			std::function<void(Particle2D&, float deltaTime)> updateFunc = defaultParticleUpdate);
		
		virtual void draw(SpriteBatch& spriteBatch, Particle2D& particle) override;

	private:
		SpriteFont m_font;	/* Font */
	};
}