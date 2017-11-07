#pragma once

class Event {
	public:
		int Play(int);
	private:
		int GenericRoom(std::string);
		// Room Events
		int Empty();
		int EntryHall();
		int Carriage();
		int Stage();
		int StageExecuted();
		int StageSpared();
		int BackStage();
		int Cave();
		int CaveClear();
		int Den();
		int DenClear();
		int Workshop();
		int CubbyHole();
		// Item Events
		int Club();
		int CursedBone();
		int Blanket();
		int Ticket();
		// Dialog Events
		int Commentator();
		// EndGame Events
		int Death();
		int Tormentor();
		int SecretEnding();
		int Victory();
		// Special Events
		int Sleep();
};