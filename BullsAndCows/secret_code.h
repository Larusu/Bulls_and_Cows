#pragma once

string generateAiSecretCode(int digitCount);
string getPlayerSecretCode(int digitCount);
bool checkSameValue(char value, string code, int index);
void printMenu();