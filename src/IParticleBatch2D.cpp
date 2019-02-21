#include "IParticleBatch2D.h"

namespace NS2 
{
	IParticleBatch2D::IParticleBatch2D() :
		m_decayRate(0),
		m_particles(nullptr),
		m_maxParticles(0),
		m_lastFreeParticle(0)
	{
	}


	IParticleBatch2D::~IParticleBatch2D()
	{
		delete[] m_particles;
	}

	void IParticleBatch2D::init(int maxParticles, float decayRate, std::function<void(Particle2D&, float deltaTime)> updateFunc)
	{
		m_maxParticles = maxParticles;
		m_decayRate = decayRate;
		m_updateFunc = updateFunc;

		m_particles = new Particle2D[m_maxParticles];
	}

	void  IParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i<m_maxParticles; i++)
		{
			if (m_particles[i].life > 0.0f)
			{
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].life -= m_decayRate;
			}
		}
	}

	void  IParticleBatch2D::draw(SpriteBatch& spriteBatch)
	{
		for (int i = 0; i<m_maxParticles; i++)
		{
			if (m_particles[i].life > 0.0f)
			{
				draw(spriteBatch, m_particles[i]);
			}
		}
	}

	void IParticleBatch2D::addParticle(const glm::vec4& destRect, const glm::vec2& velocity, 
		const ColorRGBA8& color, std::string text)
	{
		int particleIndex = findFreeParticle();
		auto& p = m_particles[particleIndex];

		p.life = 1.0f;
		p.text = text;
		p.destRect = destRect;
		p.velocity = velocity;
		p.color = color;
	}

	int IParticleBatch2D::findFreeParticle()
	{
		for (int i = m_lastFreeParticle; i<m_maxParticles; i++)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i<m_lastFreeParticle; i++)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}

		// No particles are free, so override first particle
		return 0;
	}
}