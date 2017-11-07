#include "Player.h"

Player::Player() {
	location = 0;
	health = 100;
	strength = 0;
	wisdom = 0;
	luck = 0;
}

int Player::Location(int mode, int value)
{
	switch (mode) {
	case READ: return location;
	case WRITE: location = value; return 0;
	default: break;
	}
	return -1;
}

int Player::Health(int mode, int value) {
	switch (mode) {
	case READ: return health;
	case WRITE: health = value; return 0;
	case MODIFY: health = health + value; return 0;
	default: break;
	}
	return -1;
}

int Player::Strength(int mode, int value) {
	switch (mode) {
	case READ: return strength;
	case WRITE: strength = value; return 0;
	case MODIFY: strength = strength + value; return 0;
	default: break;
	}
	return -1;
}

int Player::Wisdom(int mode, int value) {
	switch (mode) {
	case READ: return wisdom;
	case WRITE: wisdom = value; return 0;
	case MODIFY: wisdom = wisdom + value; return 0;
	default: break;
	}
	return -1;
}

int Player::Luck(int mode, int value) {
	switch (mode) {
	case READ: return luck;
	case WRITE: luck = value; return 0;
	case MODIFY: luck = luck + value; return 0;
	default: break;
	}
	return -1;
}