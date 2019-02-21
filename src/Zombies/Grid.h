#pragma once

#include "Entity.h"

#include <vector>

template<class T>
struct Cell {
	std::vector<T*> entities;
};

template<class T>
struct Cell;

template<class T>
class Grid 
{
public:
	void init(int width, int height, int cellSize);
	void clear();

	void add(T* entity, const glm::vec2& pos, unsigned int& index, Cell<T>** cell);
	void add(T* entity, unsigned int& index, Cell<T>* cell);
	void remove(unsigned int index, Cell<T>* cell);
	bool isCell(const glm::vec2& pos, int& cellX, int& cellY);

	/* GETTERS */
	int getCellSize(){ return m_cellSize; }
	int getNbCellX(){ return m_numXCells; }
	int getNbCellY(){ return m_numYCells; }
	Cell<T>& getCell(int x, int y);
	Cell<T>& getCell(const glm::vec2& pos);
	std::vector<Cell<T>>& getCells(){ return m_cells; }

private:
	std::vector<Cell<T>> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};

#include "Grid.h"

#include <cmath>
#include <cstdlib>

template<class T>
void Grid<T>::init(int width, int height, int cellSize) 
{
	m_width = width;
	m_height = height;
	m_cellSize = cellSize;
	m_numXCells = (int)ceil((float)m_width / m_cellSize);
	m_numYCells = (int)ceil((float)m_height / m_cellSize);

	// Allocate all the cells
	const int ENTITIES_TO_RESERVE = 10;
	m_cells.resize(m_numYCells * m_numXCells);
	for (size_t i = 0; i < m_cells.size(); i++) {
		m_cells[i].entities.reserve(ENTITIES_TO_RESERVE);
	}
}

template<class T>
void Grid<T>::clear() {
	m_width = 0;
	m_height = 0;
	m_cellSize = 0;
	m_numXCells = 0;
	m_numYCells = 0;
	m_cells.clear();
}

template<class T>
void Grid<T>::add(T* entity, const glm::vec2& pos, unsigned int& index, Cell<T>** cell) {
	*cell = &getCell(pos);
	index = (*cell)->entities.size();
	(*cell)->entities.push_back(entity);
}

template<class T>
void Grid<T>::add(T* entity, unsigned int& index, Cell<T>* cell) {
	index = cell->entities.size();
	cell->entities.push_back(entity);
}

template<class T>
Cell<T>& Grid<T>::getCell(int x, int y) {
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;

	return m_cells[y * m_numXCells + x];
}

template<class T>
Cell<T>& Grid<T>::getCell(const glm::vec2& pos) {
	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return getCell(cellX, cellY);
}

template<class T>
bool Grid<T>::isCell(const glm::vec2& pos, int& cellX, int& cellY) {
	cellX = (int)(pos.x / m_cellSize);
	cellY = (int)(pos.y / m_cellSize);
	return !(cellX < 0 || cellX >= m_numXCells || cellY < 0 || cellY >= m_numYCells);
}

template<class T>
void Grid<T>::remove(unsigned int index, Cell<T>* cell) {
	std::vector<T*>& entities = cell->entities;
	// Normal vector swap
	entities[index] = entities.back();
	entities.pop_back();
	// Update vector index
	if (index < entities.size())
	{
		entities[index]->setCellIndex(index);
	}
}

