#include "Map.h"

Map::Map() {
	size = 18; // Would make this variable but that requires use of vectors or pointers
}

int Map::Size(int m, int value) {
	if (m) size = value;
	return size;
}