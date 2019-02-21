#pragma once

#include <vector>
#include <string>

class LevelManager
{
public:
	void init();

private:
	unsigned int m_currentLevel;
	std::vector<std::string> m_levels;
};

