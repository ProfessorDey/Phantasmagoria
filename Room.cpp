#include "Runtime.h"
#include "Room.h"

Room::Room() {
	eventID = 0;
	north = -1;
	east = -1;
	south = -1;
	west = -1;
}

int Room::Event(int m, int value) {
	if (m == 2) return run.eventHandler.Play(eventID);
	if (m) eventID = value;
	return eventID;
}

int Room::Direction(char d, int m, int value) {
	if (m) {
		switch (d) {
			case 'N': north = value; break;
			case 'E': east = value; break;
			case 'S': south = value; break;
			case 'W': west = value; break;
			default: break;
		}
	}
	int direction = -1;
	switch (d) {
		case 'N': direction = north; break;
		case 'E': direction = east; break;
		case 'S': direction = south; break;
		case 'W': direction = west; break;
		default: break;
	}
	return direction;
	
}