#include "ParticleBatch2D.h"

namespace NS2
{
	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, 
		std::function<void(Particle2D&, float deltaTime)> updateFunc)
	{
		m_texture = texture;
		m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		IParticleBatch2D::init(maxParticles, decayRate, updateFunc);
	}

	void  ParticleBatch2D::draw(SpriteBatch& spriteBatch, Particle2D& p)
	{
		spriteBatch.draw(p.destRect, m_uvRect, m_texture.id, 0.0f, p.color);
	}
}