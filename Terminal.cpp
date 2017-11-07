#include "Game.h"
#include "Terminal.h"

Terminal::Terminal() {
	for (int i = 0; i < 3; i++) {
		fullForeground[i] = 255;
		fullBackground[i] = 0;
	}
	simpleForeground = 0;
	simpleBackground = 0;
	size[0] = 80;
	size[1] = 30;
	pos[0] = 0;
	pos[1] = 0;
}

int Terminal::SizeX() {
	return 80;
}

int Terminal::SizeY() {
	return 30;
}

int Terminal::Cursor(int x, int y) {
	_COORD cur;
	cur.X = x;
	cur.Y = y;
	if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur)) return -1;
	pos[0] = x;
	pos[1] = y;
	return 0;

}

int Terminal::GetCursor(int axis) {
	if (axis) return pos[0];
	return pos[1];
}

int Terminal::Print(std::string s) {
	std::cout << s;
	return 0;
}

int Terminal::PrintRow(std::string s, int l) {
	std::string row = "";
	for (int i = 0; i < l; i++) {
		row += s;
	}
	std::cout << row;
	return 0;
}

int Terminal::Foreground(int r, int g, int b) {
	int winColour = simpleBackground;
	switch (SimpleColour(r)) {
	case 0: break; //If Little to No Red, Do Nothing
	case 2: winColour = winColour | FOREGROUND_INTENSITY; // If a lot of Red, apply Intensity and Red
	case 1: winColour = winColour | FOREGROUND_RED; break; // If some red, apply Red
	}
	switch (SimpleColour(g)) {
	case 0: break; //If Little to No Green, Do Nothing
	case 2: winColour = winColour | FOREGROUND_INTENSITY; // If a lot of Green, apply Intensity and Green
	case 1: winColour = winColour | FOREGROUND_GREEN; break; // If some Green, apply Green
	}
	switch (SimpleColour(b)) {
	case 0: break; //If Little to No Blue, Do Nothing
	case 2: winColour = winColour | FOREGROUND_INTENSITY; // If a lot of Blue, apply Intensity and Blue
	case 1: winColour = winColour | FOREGROUND_BLUE; break; // If some Blue, apply Blue
	}
	HANDLE hTerm = GetStdHandle(STD_OUTPUT_HANDLE);
	if (SetConsoleTextAttribute(hTerm, winColour)) { // If Colour Setting Succeeds Store New Colour
		simpleForeground = winColour;
		fullForeground[0] = r;
		fullForeground[1] = g;
		fullForeground[2] = b;
		return 0; // Then return successfully
	}
	return -1; //Otherwise return failure
}

int Terminal::Background(int r, int g, int b) {
	int winColour = simpleForeground;
	winColour = winColour & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	switch (SimpleColour(r)) {
	case 0: break; //If Little to No Red, Do Nothing
	case 2: winColour = winColour | BACKGROUND_INTENSITY; // If a lot of Red, apply Intensity and Red
	case 1: winColour = winColour | BACKGROUND_RED; break; // If some red, apply Red
	}
	switch (SimpleColour(g)) {
	case 0: break; //If Little to No Green, Do Nothing
	case 2: winColour = winColour | BACKGROUND_INTENSITY; // If a lot of Green, apply Intensity and Green
	case 1: winColour = winColour | BACKGROUND_GREEN; break; // If some Green, apply Green
	}
	switch (SimpleColour(b)) {
	case 0: break; //If Little to No Blue, Do Nothing
	case 2: winColour = winColour | BACKGROUND_INTENSITY; // If a lot of Blue, apply Intensity and Blue
	case 1: winColour = winColour | BACKGROUND_BLUE; break; // If some Blue, apply Blue
	}
	HANDLE hTerm = GetStdHandle(STD_OUTPUT_HANDLE);
	if (SetConsoleTextAttribute(hTerm, winColour)) { // If Colour Setting Succeeds Store New Colour
		simpleForeground = winColour;
		fullForeground[0] = r;
		fullForeground[1] = g;
		fullForeground[2] = b;
		return 0; // Then return successfully
	}
	return -1; //Otherwise return failure
}

int Terminal::SimpleColour(int c) { // Note this assumes a maximum colour value of 255, anything higher will be treated as intense/bright
	if (c <= 85) return 0; // If up to 1/3 of 255, treat as if 0
	else if (c <= 170) return 1; // If more than 1/3 and less than 2/3, treat as regular colour
	else return 2; // If greater than 2/3, treat as a bright colour
}