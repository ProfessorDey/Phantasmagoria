#include "Game.h"
#include "Runtime.h"

Runtime run;

Runtime::Runtime() {
	sessionStart = time(0);
	running = -1;
	hiddenItem = 0;
	ticketTaken = 0;
}

int Runtime::Start() {
	int selection = 0;
	while (running) {
		int selection = mainMenu();
		switch (selection) {
		case -1: return -1;
		case 0: NewGame(); break;
		case 1: ResumeGame(); break;
		case 2: LoadGame(); break;
		case 3: SaveGame(); break;
		case 4: HelpMenu(); break;
		case 5: AboutMenu(); break;
		case 6: running = 0;  break;
		}
		run.rend.ClearScreen();
	}
	run.term.Foreground(255, 255, 255);
	run.term.Background(0, 0, 0);
	return 0;
}

int Runtime::GameLoop() {
	int playing = -1;
	while (playing) {
		int currentRoom = p.Location(0, 0);
		switch (dungeon.room[currentRoom].Event(2, 0)) {
			case -1: return -1;
			case 10: CharacterMenu(); break;
			case 99: playing = 0;
			default: break;
		}
	}
	return 0;
}

int Runtime::HiddenItem(int m, int value) {
	if (m) hiddenItem = value;
	return hiddenItem;
}

int Runtime::TicketTaken(int m, int value) {
	if (m) ticketTaken = value;
	return ticketTaken;
}