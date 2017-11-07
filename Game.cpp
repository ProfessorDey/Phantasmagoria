#include "Runtime.h"
#include "Game.h"

enum Shapes {
	NONE = 0,
	BOX = 1,
	VLINE = 2,
	HLINE = 3
};

int main() {
	return run.Start();
}

int defaultMenu() {
	if (run.rend.ClearScreen()) return -1;
	if (run.rend.addShape({ BOX, 1, 1, (run.term.SizeX() - 2), 4, 0, 0, 0, 170, 170, 170 })) return -1;
	if (run.rend.DrawText("Phantasmagoria - A Fantastical Adventure      Joshua Hayes (C) 2017 3-C BSD")) return -1;
	if (run.rend.addShape({ BOX, 2, 5, (run.term.SizeX() - 5), (run.term.SizeY() - 9), 0, 0, 0, 170, 170, 170 })) return -1;
	return 0;
}

int mainMenu() {
	if (defaultMenu()) return -1;
	if (run.rend.DrawText("Welcome Weary Traveller, to the wonderful, fantastical, magical Phantasmagoria! An adventure of wicked, twisted madness where all are at the mercy of the fair! Would you care to try you own luck in our tent of fear and mystery?                                                      [New] Game, [Resume] Game, [Load] Game, [Save] Game, [Help], [About], [Exit]")) return -1;
	if (run.rend.DrawPrompt("Please Choose An Option As Shown In []: ")) return -1;
	int selection = run.parse.Selection("New Resume Load Save Help About Exit");
	run.rend.ClearShapes();
	return selection;
}

int NewGame() {
	if (run.save.Load(-1)) return -1; // If Default Dungeon fails to load, return failure
	return ResumeGame();
}

int ResumeGame() {
	if (run.GameLoop()) return -1;
	return 0;
}

int LoadGame() {
	if (defaultMenu()) return -1;
	run.rend.DrawText(run.save.SaveSlots());
	run.rend.DrawPrompt("Select Slot to Load or [Exit]: ");
	int slot = run.parse.Selection("1 2 3 4 5 6 7 8 9 Exit");
	if (slot == 9) return 0; // If Exit, leave cleanly, otherwise continue
	if (run.save.Load(slot)) return 0; // If it fails to load, recover instead of exiting, condition here for future use/logging
	return 0;
}

int SaveGame() {
	if (defaultMenu()) return -1;
	run.rend.DrawText(run.save.SaveSlots());
	run.rend.DrawPrompt("Select Save Slot or [Exit]:");
	int slot = run.parse.Selection("1 2 3 4 5 6 7 8 9 Exit");
	if (slot == 9) return 0; // If Exit, leave cleanly, otherwise continue saving
	run.rend.DrawPrompt("Enter Name for Save: ");
	std::string name = run.parse.String();
	if (run.save.Save(slot, name)) return -1;
	return 0;
}

int HelpMenu() {
	if (defaultMenu()) return -1;
	run.rend.DrawText("Ah! Questions! But of course I'll- *Throat Clearing Sound* Right, sorry about that, that act gets a little hard on the throat. So you want help with this little adventure? Certainly. This, if you are not familiar, is a text adventure game, in the perhaps more modern fashion.                                                                                        Any time you need make a choice you will have to type your intended action into the console, you can see a basic prompt for input below, blinking away. If you enter gibberish, our blinking friend will go sulk at the top of the screen, we're still trying to train him properly, just press Enter again and he should come right on back.                                                                                              We're not completely cruel of course, nor do we have the power to predict every action you may try, so we will provide little prompts like [this] for words that you can type and get a definite response, not that we haven't hidden little extras here and there, haha. Hopefully that helps explain a few basic details. Enjoy.");
	run.rend.DrawPrompt("Press Enter To Continue: ");
	return run.parse.Enter();
}

int AboutMenu() {
	if (run.rend.ClearScreen()) return -1;
	if (run.rend.addShape({ BOX, 1, 1, (run.term.SizeX() - 2), 4, 0, 0, 0, 170, 170, 170 })) return -1;
	if (run.rend.DrawText("Phantasmagoria - A Fantastical Adventure      Joshua Hayes (C) 2017 3-C BSD")) return -1;
	if (run.rend.addShape({ BOX, 2, 5, (run.term.SizeX() - 5), 25, 0, 0, 0, 170, 170, 170 })) return -1;
	run.rend.DrawText("Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met :                                                                    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and / or other materials provided with the distribution.    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.                     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
	return run.parse.Enter();
}

int ItemMenu(int mode) {
	std::string info = "Which item would you like to ";
	std::string desc;
	std::string prompt;
	std::string itemSelection;
	int roomID = run.p.Location(0, 0);
	int slotsTaken = 0;
	int itemID = 0;
	int sel;
	switch (mode) {
		case 0: // Pickup Mode
			if (defaultMenu()) return -1;
			slotsTaken = run.dungeon.room[roomID].floor.SlotsTaken(0, 0);
			info += "take?";
			info = newLine(info, 74);
			for (int i = 0; i < slotsTaken; i++) {
				info += newLine(" ", 73);
				itemID = run.dungeon.room[roomID].floor.GetItem(i);
				desc = run.itemList[itemID].Description(0, "");
				desc[0] = toupper(desc[0]); // Capitalise first letter of description
				info += newLine(desc, 72);
				prompt += "[" + run.itemList[itemID].Name(0, "") + "] ";
				itemSelection += run.itemList[itemID].Name(0, "") + " ";
			}
			prompt = newLine(prompt, 68);
			prompt += "[Exit] ";
			prompt += "Select an item listed above with []: ";
			itemSelection += "exit";
			run.rend.DrawText(info);
			run.rend.DrawPrompt(prompt);
			sel = run.parse.Selection(itemSelection);
			if (sel == -1) return -1; // If Error in Selection, return with an error
			else if (sel >= 0 && sel < slotsTaken) {
				itemID = run.dungeon.room[roomID].floor.GetItem(sel);
				if (run.itemList[itemID].effect.Apply(0)) return 0; // If effect fails, don't pick up item. only used by event effects
				if (run.p.inventory.AddItem(itemID) || run.dungeon.room[roomID].floor.RemoveItem(sel)) return -1; // If changing over items fails, return with an error
				return 0;
			}
			else return 0;
		case 1: // Drop Mode
			if (defaultMenu()) return -1;
			slotsTaken = run.p.inventory.SlotsTaken(0, 0);
			info += "drop?";
			info = newLine(info, 74);
			for (int i = 0; i < slotsTaken; i++) {
				info += newLine(" ", 74);
				itemID = run.p.inventory.GetItem(i);
				desc = run.itemList[itemID].Description(0, "");
				desc[0] = toupper(desc[0]); // Capitalise first letter of description
				info += newLine(desc, 74);
				prompt += "[" + run.itemList[itemID].Name(0, "") + "] ";
				itemSelection += run.itemList[itemID].Name(0, "") + " ";
			}
			prompt = newLine(prompt, 68);
			prompt += "[Exit] ";
			prompt += "Select an item listed above with []: ";
			itemSelection += "exit";
			run.rend.DrawText(info);
			run.rend.DrawPrompt(prompt);
			sel = run.parse.Selection(itemSelection);
			if (sel == -1) return -1; // If Error in Selection, return with an error
			else if (sel >= 0 && sel < slotsTaken) {
				itemID = run.p.inventory.GetItem(sel);
				if (run.itemList[itemID].effect.Apply(0)) return 0; // If effect fails, don't pick up item. only used by event effects
				if (run.p.inventory.RemoveItem(sel) || run.dungeon.room[roomID].floor.AddItem(itemID)) return -1; // If changing over items fails, return with an error
			}
			else return 0;
		case 2: // Swap Mode
			return 0;
		case 3: // Use Mode (Option Accessed from character screen)
			if (defaultMenu()) return -1;
			slotsTaken = run.p.inventory.SlotsTaken(0, 0);
			info += "use?";
			info = newLine(info, 74);
			for (int i = 0; i < slotsTaken; i++) {
				info += newLine(" ", 74);
				itemID = run.p.inventory.GetItem(i);
				desc = run.itemList[itemID].Description(0, "");
				desc[0] = toupper(desc[0]); // Capitalise first letter of description
				info += newLine(desc, 74);
				prompt += "[" + run.itemList[itemID].Name(0, "") + "] ";
				itemSelection += run.itemList[itemID].Name(0, "") + " ";
			}
			prompt = newLine(prompt, 68);
			prompt += "[Exit] ";
			prompt += "Select an item listed above with []: ";
			itemSelection += "exit";
			run.rend.DrawText(info);
			run.rend.DrawPrompt(prompt);
			sel = run.parse.Selection(itemSelection);
			if (sel == -1) return -1; // If Error in Selection, return with an error
			else if (sel >= 0 && sel < slotsTaken) {
				itemID = run.p.inventory.GetItem(sel);
				if (run.itemList[itemID].effect.Apply(2)) return 0; // If effect fails, don't pick up item. only used by event effects
				return 0;
			}
			else return 0;
		default: break;
	}
	return -1;
}

std::string newLine(std::string s, int l) {
	int len = l;
	while (s.length() > len) len = len + l;
	while (s.length() < len) s += " ";
	return s;
}

int CharacterMenu() {
	if (defaultMenu()) return -1;
	std::string charSheet;
	std::string tmp;
	charSheet += newLine("Health: " + std::to_string(run.p.Health(0, 0)), 74);
	charSheet += newLine(newLine("Strength: " + std::to_string(run.p.Strength(0, 0)), 20)
						+ newLine("Wisdom: " + std::to_string(run.p.Wisdom(0, 0)), 20)
						+ newLine("Luck: " + std::to_string(run.p.Luck(0, 0)), 20),74);
	charSheet += newLine(" ", 72);
	tmp = "On your person you find ";
	if (run.p.inventory.SlotsTaken(0, 0)) {
		tmp += "the following:";
	}
	else {
		tmp += "nothing but the clothes on your back.";
	}
	charSheet += newLine(tmp, 74);
	int itemID;
	for (int i = 0; i < run.p.inventory.SlotsTaken(0,0); i++) {
		charSheet += newLine(" ", 74);
		itemID = run.p.inventory.GetItem(i);
		tmp = run.itemList[itemID].Description(0, "");
		tmp[0] = toupper(tmp[0]); // Capitalise first letter of description
		charSheet += newLine(tmp, 74);
	}
	run.rend.DrawText(charSheet);
	if (run.p.Location(0, 0) == 16) tmp = newLine("[Use] [Sleep]", 68); // If located at Cubby Hole, add sleep option
	else tmp = newLine("[Use]", 68);
	tmp += "[Back] Select an action listed above with []: ";
	run.rend.DrawPrompt(tmp);
	int sel = run.parse.Selection("use sleep back");
	switch (sel) {
		case 0: ItemMenu(3);
		case 1: if (run.p.Location(0, 0) == 16) return run.eventHandler.Play(18); // If located at Cubby Hole, return sleep event, otherwise just treat as "back"
		case 2:
		default: break;
	}
	return 0;
}