#include "Item.h"

Item::Item() {
	name = "Nothing";
	description = "";
}

std::string Item::Name(int m, std::string s) {
	if (m) name = s;
	return name;
}

std::string Item::Description(int m, std::string s) {
	if (m) description = s;
	std::string tmp = description;
	return tmp;
}