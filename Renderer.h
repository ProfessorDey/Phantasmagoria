#pragma once
class ShapeData {
	public:
		ShapeData();
		ShapeData(int, int, int, int, int, int, int, int, int, int, int);
		int mode;
		int x, y;
		int l, h;
		int fr, fg, fb;
		int br, bg, bb;
};

class Dialog {
public:
	Dialog();
	int GetIndex();
	std::string GetText();
	int SetDialog(int, std::string);
	int Clear();
private:
	int drawIndex; // The index of the "lastShape" when this text was added to the array
	std::string text; // The full text to be split and printed to fit the object it's printed to
};

class Renderer {
	public:
		Renderer();
		int DrawText(std::string); // Prints text inside the last shape drawn
		int DrawTextW(std::string); // Included to satisfy definition requirement. Presumably for Unicode Support
		int ReDraw();
		int DrawPrompt(std::string);
		int DrawNavigationPrompt();
		int addShape(ShapeData);
		int DropShape();
		int ClearScreen();
		int ClearShapes();
	private:
		size_t HashShapeData(ShapeData); // A basic, crappy hash function
		int DrawShape(ShapeData);
		int lastShape; // Contains the index of the last shape in the array
		int bufferSize; // Contains the size of the buffer
		ShapeData shape[12]; // Shape Render Call Buffer for redrawing background
		size_t shapeHash[12]; // Hash table of shape for duplicate comparison
		Dialog text[12]; // Array of text to be printed and the objects to print them to
};