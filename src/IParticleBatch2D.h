/*!
* \file IParticleBatch2D.h
* \brief Particule 2D interface.
* \author Ndongmo Silatsa Fabrice
* \date 24-02-2017
* \version 1.0
*/

#pragma once
#pragma warning(disable : 4316)

#include "Vertex.h"
#include "SpriteBatch.h"
#include "Camera2D.h"
#include "IParticleBatch2D.h"

#include <string>
#include <functional>
#include <glm\glm.hpp>

namespace NS2
{
	/*!
	* \brief Paticle 2D structure.
	*/
	class Particle2D
	{
	public:
		float life;	/* Time life */
		std::string text;
		glm::vec2 velocity;
		glm::vec4 destRect;
		ColorRGBA8 color;
	};

	/*!
	* \brief Default update method for particles.
	* \param particle :
	* \param deltatime :
	*/
	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime)
	{
		particle.destRect.x += particle.velocity.x * deltaTime;
		particle.destRect.y += particle.velocity.y * deltaTime;
	}

	class SpriteBatch;
	class IParticleBatch2D
	{
	public:
		IParticleBatch2D();
		~IParticleBatch2D();

		/*!
		* \brief Initialize particle batch stuffs.
		* \param maxParticles : number maximum of particles which can be generated
		* \param decayRate : Reduced life rate after a deltaTime
		* \param texture : particle texture
		* \param updateFunc : update particle function
		*/
		void init(int maxParticles, float decayRate, 
			std::function<void(Particle2D&, float deltaTime)> updateFunc);

		/*!
		* \brief Add particle.
		* \param destRect
		* \param velocity
		* \param color
		*/
		void addParticle(const glm::vec4& destRect, const glm::vec2& velocity, 
			const ColorRGBA8& color, std::string text = std::string());

		/*!
		* \brief Update particles.
		*/
		void update(float deltaTime);
		/*!
		* \brief Draw particles.
		*/
		void draw(SpriteBatch& spriteBatch);
		virtual void draw(SpriteBatch& spriteBatch, Particle2D& particle) = 0;

	protected:
		

		/*!
		* \brief Find free index particles in particle array. If no free index, return index of first particle (0).
		* \param free index
		*/
		int findFreeParticle();

		float m_decayRate;												/* Reduced life rate after a deltaTime */
		Particle2D* m_particles;										/* particle array */
		int m_lastFreeParticle;											/* Index of last free particle */
		int m_maxParticles;												/* Maximum number of particle */

		std::function<void(Particle2D&, float deltaTime)> m_updateFunc;	/* particle update function */
	};
}