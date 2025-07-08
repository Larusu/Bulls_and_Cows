#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "secret_code.h"

using namespace std;

string generateAiSecretCode(int digitCount) {
   srand(time(0));
   string AISecretCode = "";

   while (AISecretCode.length() < digitCount) {
       char random = (rand() % 10) + '0';

       if (AISecretCode.find(random) == string::npos) {
           AISecretCode += random;
       }
       else {
           continue;
       }
   }
   return AISecretCode;
}

string getPlayerSecretCode(int digitCount) {
    string playerSecretCode;

    while (true) {
        cout << "Enter your secret code (" << digitCount << " unique digits): ";
        cin >> playerSecretCode;

        if (playerSecretCode.length() != digitCount) {
            cout << "Invalid length. Try again.\n";
            continue;
        }

        bool allDigits = true;
        for (int i = 0; i < digitCount; i++) {
            if (playerSecretCode[i] < '0' || playerSecretCode[i] > '9') {
                allDigits = false;
                break;
            }
        }
        if (!allDigits) {
            cout << "Input must contain only digits.\n";
            continue;
        }

        bool duplicate = false;
        for (int i = 0; i < digitCount; i++) {
            if (checkSameValue(playerSecretCode[i], playerSecretCode, i)) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "Digits must be unique.\n";
            continue;
        }

        break;
    }
    return playerSecretCode;
}

bool checkSameValue(char value, string code, int index) {
    for (int i = 0; i < index; i++) {
        if (value == code[i]) {
            return true;
        }
    }
    return false;
}

void printMenu() {
	cout << endl;
    cout << "==========================================" << endl;
    cout << "        Welcome to Bulls and Cows!" << endl;
    cout << "==========================================" << endl;
    cout << "Rules: " << endl;
    cout << "1. You and the AI will each choose a 4-digit secret code." << endl;
    cout << "2. Each digit must be unique and between 0 and 9." << endl;
    cout << "3. You will try to guess the AI's code, and the AI will try to guess yours." << endl;
    cout << "4. After each guess, you'll be told how many Bulls and Cows you got:" << endl;
    cout << "   - Bulls: Correct digit in the correct position." << endl;
    cout << "   - Cows:  Correct digit in the wrong position." << endl;
    cout << "5. Each side has a maximum of 7 guesses." << endl;
    cout << "   - If neither side guesses the correct answer within 7 attempts," << endl;
    cout << "     the game ends in draw." << endl;
    cout << "Let's start!" << endl << endl;
}