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
	LevelsLoader(int num = 1) : now_level_itr(num){
		std::cout << GetCurrentLevelImage() << std::endl;
		std::cout << GetLevelDataFile() << std::endl;
	}
	std::string GetNextLevelImage() {
		now_level_itr++;
		return GetPathToLevel() + ".png";
	}
	std::string GetLevelDataFile() {
		return GetPathToLevel() + "_data.bin";
	}
	std::string GetCurrentLevelImage() {
		return GetPathToLevel() + ".png";
	}

	Point GetLevelEndPoint() {
		std::fstream stream;

		try {

			stream.open(GetLevelDataFile());
		}
		catch (std::exception e) {
			std::cout << "Error to open file " + GetLevelDataFile() << " in class LevelsLoader\n";
			e.what();
		}


	}

};