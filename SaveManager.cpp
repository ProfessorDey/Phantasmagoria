#include "SaveManager.h"
#include "Runtime.h"
#include <fstream>
#include <sstream>

SaveManager::SaveManager() {
	saveLocation = "";
}

SaveManager::SaveManager(std::string s) {
	saveLocation = s;
}

std::string SaveManager::SaveSlots() {
	std::fstream file;
	std::string line;

	std::string output = "";
	for (int i = 1; i < 10; i++) {
		file.open(saveLocation + "saveSlot" + std::to_string(i) + ".txt");
		if (!file.is_open()) {
			output += "Save Slot " + std::to_string(i) + ": Empty      ";
			continue;
		}
		getline(file, line);
		output += "Save Slot " + std::to_string(i) + ": " + line +"      ";
		file.close();
	}
	return output;
}

int SaveManager::Save(int i, std::string s) {
	std::fstream file;

	file.open(saveLocation + "saveSlot" + std::to_string(i+1) + ".txt", std::fstream::out | std::fstream::trunc);
	if (!file.is_open()) return -1;
	file << s << std::endl; // Save Name for this Save File
	file << run.p.Health(0,0) << std::endl; // Save Player Health
	file << run.p.Location(0, 0) << std::endl; // Save Player Location
	file << run.p.Strength(0,0) << " " << run.p.Wisdom(0,0) << " " << run.p.Luck(0,0) << std::endl; // Save Player Attributes
	file << run.p.inventory.SlotsTaken(0,0) << " " << run.p.inventory.Size() << std::endl; // Save Player Inventory Usage and Size
	file << run.p.inventory.ListContents() << std::endl; // Save Player Inventory
	file << std::endl; // Break in contents for debugging and human legibility of Save File
	file << run.dungeon.Size(0,0) << std::endl; // Save Dungeon Size For Loading Map
	for (int i = 0; i < run.dungeon.Size(0,0); i++) {
		file << run.dungeon.room[i].Event(0,0) << std::endl;
		file << run.dungeon.room[i].Direction('N',0,0) << " " << run.dungeon.room[i].Direction('E',0,0) << " " << run.dungeon.room[i].Direction('S',0,0) << " " << run.dungeon.room[i].Direction('W',0,0) << std::endl;
		file << run.dungeon.room[i].floor.SlotsTaken(0,0) << " " << run.dungeon.room[i].floor.Size() << std::endl; // Save Room Inventory Usage and Size
		file << run.dungeon.room[i].floor.ListContents() << std::endl;
	}
	file << std::endl; // Seperator between dungeon data and itemlist
	for (int i = 0; i < 64; i++) {
		file << run.itemList[i].Name(0, "") << std::endl; // Store Item Name
		file << run.itemList[i].Description(0, "") << std::endl; // Store Item Description
		file << run.itemList[i].effect.Effects(0, "") << std::endl; // Store Item Effects
	}
	file.close(); // Saving Complete, close fstream
	return 0;
}

int SaveManager::Load(int i) {
	std::fstream file;
	std::string line;

	if (i == -1) file.open("defaultDungeon.txt"); // If given save slot -1 to load, load the default scenario/map
	else file.open(saveLocation + "saveSlot" + std::to_string(i + 1) + ".txt", std::fstream::in);
	if (!file.is_open()) return -1;
	getline(file, line); // Bypass the save name
	getline(file, line); // Retrieve Player Health
	run.p.Health(1, stoi(line)); // Write Player Health to Runtime Environment
	getline(file, line); // Get Player Location
	run.p.Location(1, stoi(line));
	getline(file, line); // Get Player Attributes
	{ // Encapsulated string streaming, just because it's neater
		std::istringstream is(line);
		int s, w, l;
		is >> s >> w >> l;
		run.p.Strength(1, s);
		run.p.Wisdom(1, w);
		run.p.Luck(1, l);
	}
	{
		getline(file, line);
		std::istringstream is(line);
		int sT, s;
		is >> sT >> s;
		if (sT > s || s != run.p.inventory.Size()) return -1;
		run.p.inventory.SlotsTaken(1, sT);
	}
	getline(file, line); // Get Player Inventory Contents
	run.p.inventory.SetContents(line); //Set Player Inventory Contents
	getline(file, line); // Skip Empty line below player entries
	getline(file, line); // Get Map Size
	run.dungeon.Size(1, stoi(line));
	for (int i = 0; i < run.dungeon.Size(0, 0); i++) {
		getline(file, line);
		run.dungeon.room[i].Event(1, stoi(line));
		{
			getline(file, line);
			std::istringstream is(line);
			int n, e, s, w;
			is >> n >> e >> s >> w;
			run.dungeon.room[i].Direction('N', 1, n);
			run.dungeon.room[i].Direction('E', 1, e);
			run.dungeon.room[i].Direction('S', 1, s);
			run.dungeon.room[i].Direction('W', 1, w);
		}
		{
			getline(file, line);
			std::istringstream is(line);
			int sT, s;
			is >> sT >> s;
			if (sT > s || s != run.dungeon.room[i].floor.Size()) return -1;
			run.dungeon.room[i].floor.SlotsTaken(1, sT);
		}
		getline(file, line);
		run.dungeon.room[i].floor.SetContents(line);
	}
	getline(file, line); // Bypass buffer line
	for (int i = 0; i < 64; i++) {
		getline(file, line); // Get Item Name
		run.itemList[i].Name(1, line); // Set Item Name
		getline(file, line); // Get Item Description
		run.itemList[i].Description(1, line); // Set Item Description
		getline(file, line); // Get Item Effects
		run.itemList[i].effect.Effects(1, line); // Set Item Effects
	}
	file.close();
	return 0;
}

