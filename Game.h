#pragma once
#include <stdio.h>
#include <string>
#include <conio.h>

int main();
int defaultMenu();
int mainMenu();
int NewGame();
int ResumeGame();
int LoadGame();
int SaveGame();
int HelpMenu();
int AboutMenu();
std::string newLine(std::string, int);
int ItemMenu(int);
int CharacterMenu();