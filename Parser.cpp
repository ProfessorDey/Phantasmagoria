#include "Parser.h"
#include "Runtime.h"
#include <iostream>
#include <algorithm>

int Parser::Selection(std::string s) {
	int pos[2] = { run.term.GetCursor(0), run.term.GetCursor(1) };
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	std::string options[10];
	for (int i = 0; i < 10; i++) {
		options[i] = "";
	}
	int index = 0;
	int offset = 0;
	while (index < 10) {
		while (s[offset] != ' ') {
			if (offset == s.length()) {
				options[index] = s;
				index = 10;
				break;

			}
			else offset++;
		}
		if (index == 10) break;
		options[index] = s.substr(0, offset);
		s = s.substr(offset + 1, s.length());
		offset = 0;
		index++;
	}
	index = 0;
	std::string input = "";
	int valid = 0;
	while (!valid) {
		getline(std::cin, input);
		run.term.Cursor(pos[0], pos[1]);
		std::transform(input.begin(), input.end(), input.begin(), ::tolower);
		while (index < 10 && input != options[index]) index++;
		if (index == 10) {
			index = 0;
		}
		if (input == options[index]) valid = !valid;
	}
	return index;


}

int Parser::Enter() {
	getchar();
	return 0;
}


std::string Parser::String() {
	std::string s;
	getline(std::cin, s);
	return s;
}
