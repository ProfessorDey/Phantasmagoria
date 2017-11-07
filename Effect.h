#pragma once

class Effect {
public:
	Effect();
	int Apply(int);
	std::string Effects(int, std::string);
private:
	int PlayerMod(int, int);
	int pickup[2];
	int drop[2];
	int use[2];
};