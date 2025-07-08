#pragma once

void generateAiSecretCode(int digitCount);
void getPlayerSecretCode(int digitCount);
bool checkSameValue(char value, std::string code, int index);
void printMenu(int length);
void startGame();