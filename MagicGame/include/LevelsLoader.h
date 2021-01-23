#pragma once
#include <string>
#include <fstream>
#include "Grounds.h"

class LevelsLoader {
private:
	int now_level_itr;
	std::string path_ = "levels/";

	std::string GetPathToLevel() {
		return (path_ + "level_" + std::to_string(now_level_itr) + "/Level" + std::to_string(now_level_itr));
	}


public:
	LevelsLoader(int num = 1) : now_level_itr(num) {	}

	void IncreaseLevelCounter() {
		now_level_itr++;
	}

	std::string GetLevelDataFile() {
		return GetPathToLevel() + "_data.bin";
	}

	std::string GetCurrentLevelImage() {
		return GetPathToLevel() + ".png";
	}

};