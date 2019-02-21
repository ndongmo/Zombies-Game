#include "LevelManager.h"

#include <IOManager.h>
#include <algorithm>

void LevelManager::init()
{
	int last, min;
	std::string tmp, nb;
	const std::string delim = "_", dir = "Levels";
	std::vector<NS2::DirEntry>rvEntries;
	NS2::IOManager::getDirectoryEntries(dir.c_str(), rvEntries);

	for (size_t i = 0; i < rvEntries.size(); i++) {
		if (!rvEntries[i].isDirectory) {
			m_levels.push_back(rvEntries[i].path);
		}
	}

	std::sort(m_levels.begin(), m_levels.end(), [&delim, &dir](const std::string& s1, const std::string& s2){
		std::string tmp1 = s1, tmp2 = s2;
		int v1 = 0, v2 = 0;

		try{
			tmp1.erase(0, dir.size() + 1);
			tmp1 = tmp1.substr(0, tmp1.find(delim));
			v1 = std::stoi(tmp1);

			tmp2.erase(0, dir.size() + 1);
			tmp2 = tmp2.substr(0, tmp2.find(delim));
			v2 = std::stoi(tmp2);

			return v1 < v2;
		}
		catch (...){}
		return false;
	});
}
