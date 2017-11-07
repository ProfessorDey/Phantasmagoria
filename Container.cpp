#include "Container.h"
#include <sstream>

Container::Container() {
	size = 6;
	slotsTaken = 0;
	for (int i = 0; i < size; i++) {
		contents[i] = 0;
	}
}

int Container::SlotsTaken(int m, int value) {
	if (m) slotsTaken = value;
	return slotsTaken;
}

int Container::Size() {
	return size;
}

std::string Container::ListContents() {
	std::string contentList = "";
	for (int i = 0; i < size; i++) {
		contentList += std::to_string(contents[i]);
		if (i != (size - 1)) contentList += " ";
	}
	return contentList;
}

int Container::GetItem(int index) {
	return contents[index];
}

int Container::SetContents(std::string s) {
	std::istringstream is(s);
	for (int i = 0; i < size; i++) {
		is >> contents[i];
	}
	return 0;
}

int Container::AddItem(int itemID) {
	if (slotsTaken < size) {
		contents[slotsTaken] = itemID;
		slotsTaken++;
		return 0;
	}
	return -1;
}

int Container::RemoveItem(int index) {
	if (contents[index]) contents[index] = 0; // If container is not empty, remove contents
	else return -1;
	slotsTaken--; // Decrement slotsTaken
	if (index != slotsTaken) Sort(); // Then if index does not match slotsTaken (slotsTaken being 1 higher than highest filled slot until we decremented it), sort, as we have left an empty slot below slotsTaken
	return 0;
}

int Container::Sort() { // A simple filling sorting function, if open slot, fill it with context of highest non-empty index or finish if none available
	int sorting = -1;
	int index = 0;
	int backIndex = size -1;
	while (index < size) {
		if (!contents[index]) { // If slot is empty
			while (backIndex != index && !contents[backIndex]) {
				backIndex--;
			}
			if (backIndex == index) break; // If no items beyond index to sort, break loop and return
			contents[index] = contents[backIndex]; // Otherwise shift contents
			contents[backIndex] = 0;
		}
		else index++; // Otherwise keep iterating through slots
	}
	return 0;
}