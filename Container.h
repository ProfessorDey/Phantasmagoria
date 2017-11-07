#pragma once
#include "Game.h"

class Container {
	public:
		Container();
		int SlotsTaken(int, int);
		int Size();
		std::string ListContents();
		int GetItem(int);
		int SetContents(std::string);
		int AddItem(int);
		int RemoveItem(int);
	private:
		int Sort();
		int size;
		int slotsTaken;
		int contents[6];
};

