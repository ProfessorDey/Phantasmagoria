#pragma once
#include "Container.h"
enum Mode {
	READ = 0,
	WRITE = 1,
	MODIFY = 2
};

class Player {
	public:
		Player();
		int Location(int, int);
		int Health(int, int);
		int Strength(int, int);
		int Wisdom(int, int);
		int Luck(int, int);
		Container inventory;
	private:
		int location;
		int health;
		int strength;
		int wisdom;
		int luck;
};
