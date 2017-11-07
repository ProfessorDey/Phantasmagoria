#pragma once
#include "Game.h"
#include "Effect.h"

class Item {
	public:
		Item();
		std::string Name(int, std::string);
		std::string Description(int, std::string);
		Effect effect;
	private:
		std::string name;
		std::string description;
};