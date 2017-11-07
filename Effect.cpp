#include "Runtime.h"
#include "Effect.h"
#include <sstream>

enum Attributes {
	HEALTH = 0,
	LOCATION = 1,
	STRENGTH = 2,
	WISDOM = 3,
	LUCK = 4,
	EVENT = 5
};

Effect::Effect() {
	for (int i = 0; i < 2; i++) {
		pickup[i] = -1;
		drop[i] = -1;
		use[i] = -1;
	}
}

int Effect::Apply(int m) {
	switch (m) {
	case 0: PlayerMod(pickup[0], pickup[1]); break;
	case 1: PlayerMod(drop[0], drop[1]); break;
	case 2: PlayerMod(use[0], use[1]); break;
	default: return -1;
	}
	return 0;
}

std::string Effect::Effects(int m, std::string s)
{
	if (m) {
		std::istringstream is(s);
		is >> pickup[0] >> pickup[1] >> drop[0] >> drop[1] >> use[0] >> use[1];
		return s;
	}
	else {
		std::string out = std::to_string(pickup[0])
				  + " " + std::to_string(pickup[1])
				  + " " + std::to_string(drop[0])
				  + " " + std::to_string(drop[1])
				  + " " + std::to_string(use[0])
				  + " " + std::to_string(use[1]);
		return out;
	}
}

int Effect::PlayerMod(int m, int value) {
	switch (m) {
		case HEALTH: run.p.Health(2, value); break;
		case LOCATION: run.p.Location(1, value); break;
		case STRENGTH: run.p.Strength(2, value); break;
		case WISDOM: run.p.Wisdom(2, value); break;
		case LUCK: run.p.Luck(2, value); break;
		case EVENT:  run.eventHandler.Play(value); break;
		default: break;
	}
	return 0;
}