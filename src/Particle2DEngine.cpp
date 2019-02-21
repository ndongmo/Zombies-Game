#include "Particle2DEngine.h"
#include "IParticleBatch2D.h"
#include "SpriteBatch.h"
#include "Camera2D.h"

namespace NS2
{
	Particle2DEngine::Particle2DEngine(void)
	{
	}


	Particle2DEngine::~Particle2DEngine(void)
	{
		/*for (auto& pb : m_particleBatches)
		{
			delete pb;
		}*/
	}

	void Particle2DEngine::addParticleBatch(IParticleBatch2D* particleBatch)
	{
		m_particleBatches.push_back(particleBatch);
	}

	void Particle2DEngine::update(float deltaTime)
	{
		for (auto& pb : m_particleBatches)
		{
			pb->update(deltaTime);
		}
	}

	void Particle2DEngine::draw(SpriteBatch& spriteBatch)
	{
		for (auto& pb : m_particleBatches)
		{
			spriteBatch.begin();
			{
				pb->draw(spriteBatch);
			}
			spriteBatch.end();
			spriteBatch.renderBatch();
		}
	}
}