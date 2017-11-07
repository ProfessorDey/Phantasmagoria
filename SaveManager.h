#pragma once
#include "Game.h"

class SaveManager {
	public:
		SaveManager();
		SaveManager(std::string);
		std::string SaveSlots();
		int Save(int, std::string);
		int Load(int);
	private:
		std::string saveLocation;
};