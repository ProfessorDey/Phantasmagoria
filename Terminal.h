#pragma once
#include <Windows.h>
#include <iostream>

class Terminal {
	public:
		Terminal();
		int SizeX();
		int SizeY();
		int Cursor(int, int);
		int GetCursor(int);
		int Print(std::string);
		int PrintRow(std::string, int);
		int Foreground(int, int, int); 
		int Background(int, int, int);
	private:
		int SimpleColour(int);

		int fullForeground[3]; // Full Foreground Colour (r, g, b)
		int fullBackground[3]; // Full Background Colour (r, g, b)
		int simpleForeground; // Simple Windows Compatible Foreground Colour Flags
		int simpleBackground; // Simple Windows Compatible Background Colour Flags

		int size[2]; // Terminal Size (x, y)
		int pos[2]; // Pointer Location (x, y)
		// GetConsoleScreenBuffer requires a pointer, so we will have to operate blindly
};