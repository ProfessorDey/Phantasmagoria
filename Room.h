#pragma once
#include "Container.h"

class Room {
	public:
		Room();
		int Event(int, int);
		int Direction(char, int, int);
		Container floor;
	private:
		int eventID;
		int eventCleared;
		int north, east, south, west;
};