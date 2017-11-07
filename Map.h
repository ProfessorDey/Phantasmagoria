#pragma once
#include "Room.h"

class Map {
	public:
		Map();
		int Size(int, int);
		Room room[18];
	private:
		int size;
};

