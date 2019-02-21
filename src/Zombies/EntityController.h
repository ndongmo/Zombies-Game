/*!
* \file EntityController.h
* \brief Controller class of agents' update and agents' drawing.
* \author Ndongmo Silatsa Fabrice
* \date 26-12-2016
* \version 1.0
*/

#pragma once

#include "Grid.h"

#include <vector>

class Level;

class EntityController
{
public:
	/*!
	* \brief Update the agents.
	* \param agents: the agents to update
	*/
	void updateEntities(Level& level, float deltaTime);
	/* Some simple event functions */
	void onMouseDown(std::vector<Entity>& entities, float mouseX, float mouseY);

private:
	/*!
	* \brief Updates collision.
	* \param grid: grid of agents
	*/
	void updateCollision(Level& level);
	/*!
	* \brief Checks collision between an agent and a vector of agents,
	*	starting at a specific index.
	*/
	void checkCollision(Level& level, Entity* entity, std::vector<Entity*>& entitiesToCheck, int startingIndex);
};

