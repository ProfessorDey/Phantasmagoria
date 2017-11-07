#include "Runtime.h"
#include "Renderer.h"

enum Shapes {
	NONE = 0,
	BOX = 1,
	VLINE = 2,
	HLINE = 3
};

// --------------------------------------------------

ShapeData::ShapeData() {
	mode = 0;
	x, y = 0;
	l, h = 0;
	fr, fg, fb = 0;
	br, bg, bb = 0;
}

ShapeData::ShapeData(int M, int X, int Y, int L, int H, int FR, int FG, int FB, int BR, int BG, int BB) {
	mode = M;
	x = X;	y = Y;
	l = L;	h = H;
	fr = FR;	fg = FG;	fb = FB;
	br = BR;	bg = BG;	bb = BB;
}

// --------------------------------------------------

Dialog::Dialog() {
	drawIndex = -1;
	text = "";
}

int Dialog::GetIndex() {
	return drawIndex;
}

std::string Dialog::GetText() {
	return text;
}

int Dialog::SetDialog(int i, std::string s) {
	drawIndex = i;
	text = s;
	return 0;
}

int Dialog::Clear() {
	drawIndex = -1;
	text = "";
	return 0;
}

// --------------------------------------------------

Renderer::Renderer() {
	bufferSize = 12;
	lastShape = 0;
}

int Renderer::DrawText(std::string s) { // Prints text inside the last shape drawn
	{
		int textIndex = 0;
		while (textIndex < 12 && text[textIndex].GetIndex() != -1) textIndex++;
		text[textIndex].SetDialog(lastShape, s);
	}
	int messageSpace = shape[lastShape].l - 2; // Find size that text can be fitted into
	int line = 1;
	while (s.length()) { // While String is not empty
		if (run.term.Cursor((shape[lastShape].x + 1), (shape[lastShape].y + line))) return -1; // Position Cursor for printing
		if (s.length() > messageSpace) { // Check if string fits within boundries
			int offset = 0;
			while (messageSpace - offset > 0 && s[messageSpace - offset] != ' ') offset++;
			if (messageSpace == offset) { // If there are no spaces to be found, cut word midway
				offset = 0;
			}
			std::string tmp = s.substr(0, messageSpace - offset); // Create substring that fits constraints
			if (run.term.Print(tmp)) return -1; // Print substring
			s = s.substr((messageSpace - offset + 1), s.length()); // Modify remaining string to drop printed section
			line++;
		}
		else if (s.length() < messageSpace) {
			if (run.term.Print(s))return -1; // Otherwise print string
			s = ""; // Clear string to exit loop
		}
		else return -1;
	}
	return 0;
}

int Renderer::DrawPrompt(std::string s) {
	if (addShape(
		{	BOX, 
			1, 
			(run.term.SizeY() - 4), 
			(run.term.SizeX() - 3), 
			5, 
			0, 0, 0, 
			170, 170, 170 })) return -1; // Add Box for prompt
	if (run.rend.DrawText(s)) return -1; // Print Prompt Text
	return 0;
}

int Renderer::DrawNavigationPrompt() {
	if (addShape(
	{ BOX,
		1,
		(run.term.SizeY() - 4),
		(run.term.SizeX() - 3),
		5,
		0, 0, 0,
		170, 170, 170 })) return -1; // Add Box for prompt
	run.term.Cursor(2, (run.term.SizeY() - 3));
	char dir = 'N';
	std::string direction = "North";
	int roomID = run.p.Location(0, 0);
	int colour[3] = { 0,0,0 };
	for (int i = 0; i < 4; i++) {
		switch (i) {
			case 1: direction = "West"; break;
			case 2: direction = "South"; break;
			case 3: direction = "East"; break;
			default: break;
		}
		dir = direction[0];
		if (run.dungeon.room[roomID].Direction(dir,0,0) == -1) {
			colour[0] = 255;
			colour[1] = 255;
			colour[2] = 255;
		}
		else {
			colour[0] = 0;
			colour[1] = 0;
			colour[2] = 0;
		}
		run.term.Foreground(colour[0], colour[1], colour[2]);
		run.term.Background(170,170,170);
		run.term.Print("[" + direction + "] ");
	}
	if (!run.dungeon.room[roomID].floor.SlotsTaken(0, 0)) { // If there are no items on the floor
		colour[0] = 255;
		colour[1] = 255;
		colour[2] = 255;
	}
	else {
		colour[0] = 0;
		colour[1] = 0;
		colour[2] = 0;
	}
	run.term.Foreground(colour[0], colour[1], colour[2]);
	run.term.Background(170, 170, 170);
	run.term.Print("[Take] ");
	if (!run.p.inventory.SlotsTaken(0, 0) || !run.dungeon.room[roomID].floor.SlotsTaken(0, 0)) { // If no items in either player or room inventories, blank out [Swap]
		colour[0] = 255;
		colour[1] = 255;
		colour[2] = 255;
	}
	else {
		colour[0] = 0;
		colour[1] = 0;
		colour[2] = 0;
	}
	run.term.Foreground(colour[0], colour[1], colour[2]);
	run.term.Background(170, 170, 170);
	run.term.Print("[Swap] ");
	if (!run.p.inventory.SlotsTaken(0, 0) || run.dungeon.room[roomID].floor.SlotsTaken(0, 0) == run.dungeon.room[roomID].floor.Size()) {
		colour[0] = 255;
		colour[1] = 255;
		colour[2] = 255;
	}
	else {
		colour[0] = 0;
		colour[1] = 0;
		colour[2] = 0;
	}
	run.term.Foreground(colour[0], colour[1], colour[2]);
	run.term.Background(170, 170, 170);
	run.term.Print("[Drop] ");
	run.term.Foreground(0, 0, 0);
	run.term.Background(170, 170, 170);
	run.term.Print("[Character] ");
	run.term.Foreground(0,0,0);
	run.term.Background(170, 170, 170);
	run.term.Print("[Exit]");
	run.term.Cursor(2, (run.term.SizeY() - 2));
	run.term.Print("Select an action listed above with []: ");
	return 0;
}

int Renderer::ReDraw() {
	for (int i = 0; i < lastShape; i++) {
		if (DrawShape(shape[i])) return -1;
	}
	int storage = lastShape;
	for (int i = 0; i < 12; i++) {
		if (text[i].GetIndex() == -1) continue; // If no text in that slot, skip ahead
		lastShape = text[i].GetIndex(); // A rather poor solution for this, but should still work fine
		if (DrawText(text[i].GetText())) return -1; // As long as we remove text that's linked to removed shapes
	}
	lastShape = storage;

	return 0;
}

int Renderer::addShape(ShapeData s) {
	size_t sHash = HashShapeData(s);
	if (shapeHash[0]) { // If the first item in the array is not empty...
		int dup = 0;
		while (dup <= lastShape && !(shapeHash[dup] == sHash)) { // Check hash array for duplicates
			dup++;
		}
		if (dup > lastShape) { // if no duplicates found...
			if (lastShape == bufferSize) return -1; // Return -1 if no more space in buffer
			lastShape++; // Otherwise increment lastShape
		}
		else { // If a duplicate _is_ found...
			for (; dup < lastShape; dup++) { // Then shift each item after the duplicate back one place
				shape[dup] = shape[dup + 1]; // For both shapes and shape hashes
				shapeHash[dup] = shapeHash[dup + 1];
			}
		}
	}
	shape[lastShape] = s; // Then add shape and shape hash to appropriate location
	shapeHash[lastShape] = sHash;
	DrawShape(s); // Finally draw shape to screen
	return 0;
}

int Renderer::DropShape() {
	ShapeData empty;
	shape[lastShape] = empty;
	shapeHash[lastShape] = 0;
	for (int i = 0; i < 12; i++) {
		if (text[i].GetIndex() == lastShape) text[i].Clear();
	}
	if (lastShape) lastShape--; // Check if index is not for first element (0), if not decrement lastShape. Used to prevent negative index
	return 0;
}

int Renderer::ClearScreen() {
	run.term.Foreground(255, 255, 255);
	run.term.Background(0, 0, 0);
	for (int i = 0; i < run.term.SizeY(); i++) {
		run.term.Cursor(0, i);
		run.term.PrintRow(" ", run.term.SizeX());
	}
	ClearShapes();
	return 0;
}

int Renderer::ClearShapes() {
	while (lastShape) {
		if (DropShape()) return -1;
	}
	for (int i = 0; i < 12; i++) text[i].Clear();
	return 0;
}

size_t Renderer::HashShapeData(ShapeData s) { // A basic, crappy hash function
	size_t h = 0;
	h = h + s.mode + s.l + s.h;
	h = h * s.fr * s.fg * s.fb;
	h = h + s.br + s.bg + s.bb + s.x + s.y;
	return h;
}

int Renderer::DrawShape(ShapeData s) { // Takes Shape Data
	if (s.mode == NONE) return 0; // If empty, ignore all other values and return

	if (run.term.Cursor(s.x, s.y)) return -1; // Set Cursor Position
	if (run.term.Foreground(s.fr, s.fg, s.fb)) return -1; // Set Foreground & Background
	if (run.term.Background(s.br, s.bg, s.bb)) return -1;
	switch (s.mode) {
		case BOX:
			for (int i = 0; i < s.h; i++) {
				if (run.term.PrintRow(" ", s.l)) return -1;
				if (run.term.Cursor(s.x, s.y++)) return -1;
			}
			return 0;

		case VLINE:
			for (int i = 0; i < s.h; i++) {
				if (run.term.Print(" ")) return -1;
				if (run.term.Cursor(s.x, s.y++)) return -1;
			}
			return 0;

		case HLINE:
			if (run.term.PrintRow(" ", s.l)) return -1;
			return 0;
		default:
			break;
	}
	return -1;
}