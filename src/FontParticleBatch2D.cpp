#include "FontParticleBatch2D.h"

namespace NS2
{
	void FontParticleBatch2D::init(int maxParticles, float decayRate, SpriteFont& font,
		std::function<void(Particle2D&, float deltaTime)> updateFunc)
	{
		m_font = font;
		IParticleBatch2D::init(maxParticles, decayRate, updateFunc);
	}

	void  FontParticleBatch2D::draw(SpriteBatch& spriteBatch, Particle2D& p)
	{
		m_font.draw(spriteBatch, p.text.c_str(), p.destRect, 0.0f, p.color);
	}
}