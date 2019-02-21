#include "EntityController.h"
#include "Level.h"
#include "Grid.h"

void EntityController::updateEntities(Level& level, float deltaTime) {
	for (size_t i = 0; i < level.entities.size(); i++)
	{
		Entity* entity = level.entities[i];
		assert(entity->getCell() != nullptr);

		if (entity->getState() == EntityState::DYNAMIC)
		{
			// Upadate entity
			entity->update(level, deltaTime);
			// Check to see if the entity moved
			Cell<Entity>* newCell = &level.grid.getCell(entity->getShape().getCenter());
			if (newCell != entity->getCell()) {
				unsigned int index = 0;
				level.grid.remove(level.entities[i]->getCellIndex(), level.entities[i]->getCell());
				level.grid.add(level.entities[i], index, newCell);
				level.entities[i]->setCellIndex(index);
				level.entities[i]->setCell(newCell);
			}

			entity->checkCollision(level);
		}

		if (!entity->isAlive())
			level.toRemove.push_back(entity);
	}

	// always before add entities to world
	for (unsigned int i = 0; i < level.toRemove.size(); i++){
		Entity* entity = level.toRemove[i];
		level.removeFromWorld(entity);
		entity->destroy(level);
	}

	for (unsigned int i = 0; i < level.toAdd.size(); i++){
		Entity* entity = level.toAdd[i];
		level.addToWorld(entity);
	}

	if (!level.toAdd.empty()) level.toAdd.clear();
	if (!level.toRemove.empty()) level.toRemove.clear();

	// Updates all collisions using the spatial partitioning
	updateCollision(level);
}

void EntityController::onMouseDown(std::vector <Entity>& entities, float mouseX, float mouseY) {

}

void EntityController::updateCollision(Level& level)
{
	for (size_t i = 0; i < level.grid.getCells().size(); i++)
	{
		int x = i % level.grid.getNbCellX();
		int y = i / level.grid.getNbCellX();

		Cell<Entity>& cell = level.grid.getCells()[i];

		// Loop through all entities in a cell
		for (size_t j = 0; j < cell.entities.size(); j++)
		{
			Entity* entity = cell.entities[j];
			/// Update with the residing cell
			checkCollision(level, entity, cell.entities, j + 1);

			/// Update collision with neighbor cells
			if (x > 0) {
				// Left
				checkCollision(level, entity, level.grid.getCell(x - 1, y).entities, 0);
				if (y > 0) {
					/// Top left
					checkCollision(level, entity, level.grid.getCell(x - 1, y - 1).entities, 0);
				}
				if (y < level.grid.getNbCellY() - 1) {
					// Bottom left
					checkCollision(level, entity, level.grid.getCell(x - 1, y + 1).entities, 0);
				}
			}
			// Up cell
			if (y > 0) {
				checkCollision(level, entity, level.grid.getCell(x, y - 1).entities, 0);
			}
		}
	}
}

void EntityController::checkCollision(Level& level, Entity* entity, std::vector<Entity*>& entitiesToCheck, int startingIndex)
{
	for (size_t i = startingIndex; i < entitiesToCheck.size() && entity != entitiesToCheck[i]; i++)
	{
		glm::vec2 collisionDepth;
		if (entity->getShape().collideWith(entitiesToCheck[i]->getShape(), collisionDepth))
		{
			if (entitiesToCheck[i]->getState() == EntityState::DYNAMIC)
			{
				entity->collide(level, entitiesToCheck[i], collisionDepth / 2.0f);
				entitiesToCheck[i]->collide(level, entity, -collisionDepth / 2.0f);
			}
			else
			{
				entity->collide(level, entitiesToCheck[i], collisionDepth);
			}
		}
	}
}