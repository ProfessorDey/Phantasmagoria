#pragma once
#include <time.h>
#include <string>
#include "Event.h"
#include "Map.h"
#include "Player.h"
#include "Renderer.h"
#include "Terminal.h"
#include "Parser.h"
#include "SaveManager.h"
#include "Item.h"
#include "Effect.h"

class Runtime {
	public:
		Runtime();
		int Start();
		int GameLoop();
		int HiddenItem(int, int);
		int TicketTaken(int, int);
		Map dungeon;
		Player p;
		Event eventHandler;
		Renderer rend;
		Terminal term;
		Parser parse;
		SaveManager save;
		Item itemList[64];
	private:
		int running;
		int hiddenItem;
		int ticketTaken;
		time_t sessionStart;
};

extern Runtime run;