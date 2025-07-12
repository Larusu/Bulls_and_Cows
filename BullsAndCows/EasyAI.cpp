//
// Bulls and Cows game
// Easy AI
// By Jhervis Arevalo and Lars Timajo
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

string generateAiSecretCode(int digitCount);
string getPlayerSecretCode(int digitCount);
void printMenu(int length);
void setupGame();
bool validationDigitAndLength(string value, int length);
void startGame(int length);
int countBullCow(string input, string secretCode, string returnType);

int main()
{
    srand(time(0));
    setupGame();
}

void setupGame() {

    int codeLength = 0;

    while (true) {
        cout << "Choose secret code length: ";
        cin >> codeLength;
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Must be a number. \n";
            codeLength = 0;
            continue;
        }
        cin.ignore();

        if (codeLength > 9 || codeLength < 4) {
            cout << "Invalid input! Length must only be 4 - 9." << endl;
            continue;
        }
        break;
    }

    printMenu(codeLength);
    startGame(codeLength);
}

void startGame(int length) {

    string aiSecretCode = generateAiSecretCode(length);
    string playerSecretCode = getPlayerSecretCode(length);

    string playerGuess, aiGuess;
    int count = 1;
    int bullPlayer, cowPlayer, bullAi, cowAi;

    while (true) {
        cout << "=============================" << endl;
        cout << "Guess #" << count << endl;
        cout << "Player Guess: ";
        cin >> playerGuess; cin.ignore();

        if (!validationDigitAndLength(playerGuess, length)) continue;

        aiGuess = generateAiSecretCode(length);
        cout << "AI Guess: " << aiGuess;

        bullPlayer = countBullCow(playerGuess, aiSecretCode, "bull");
        cowPlayer = countBullCow(playerGuess, aiSecretCode, "cow");
        bullAi = countBullCow(aiGuess, playerSecretCode, "bull");
        cowAi = countBullCow(aiGuess, playerSecretCode, "cow");

        cout << "\nPlayer's Bull: " << bullPlayer << endl;
        cout << "Player's Cow: " << cowPlayer << endl;
        cout << "Ai's Bull: " << bullAi << endl;
        cout << "Ai's Cow: " << cowAi << endl;

        if (bullAi == 4) {
            cout << "\nUnfortunately, the Ai won!" << endl;
            break;
        }
        else if (bullPlayer == 4) {
            cout << "\nCongratulations! You won!" << endl;
            break;
        }
        else if (count == 7) {
            cout << "\nNobody wins. The game is draw!" << endl;
            break;
        }
        count++;
    }
    cout << "Player's Secret Code: " << playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << aiSecretCode << endl;
}

int countBullCow(string input, string secretCode, string returnType) {

    int bull = 0, cow = 0;
    for (int i = 0; i < input.length(); i++) {
        for (int j = 0; j < secretCode.length(); j++) {
            if (i == j && input[i] == secretCode[j]) {
                bull++;
                break;
            }
            else if (input[i] == secretCode[j]) {
                cow++;
                break;
            }
        }
    }
    if (returnType == "bull") return bull;
    return cow;
}

string generateAiSecretCode(int digitCount) {

    string AISecretCode = "";

    while (AISecretCode.length() < digitCount) {
        char random = (rand() % 10) + '0';

        if (AISecretCode.find(random) == string::npos) {
            AISecretCode += random;
        }
    }
    return AISecretCode;
}

string getPlayerSecretCode(int digitCount) {
    string playerSecretCode;

    while (true) {
        cout << "Enter your secret code (" << digitCount << " unique digits): ";
        cin >> playerSecretCode; cin.ignore();

        if (!validationDigitAndLength(playerSecretCode, digitCount)) continue;

        bool duplicate = false;
        for (int i = 0; i < digitCount; i++) {
            for (int j = 0; j < i; j++) {
                if (playerSecretCode[i] == playerSecretCode[j]) {
                    duplicate = true;
                    break;
                }
            }
        }
        if (duplicate) {
            cout << "Digits must be unique.\n";
            continue;
        }
        return playerSecretCode;
    }
}

bool validationDigitAndLength(string value, int length) {

    if (value.length() != length) {
        cout << "Invalid length. Try again.\n";
        return false;
    }
    for (int i = 0; i < value.length(); i++) {
        if (value[i] < '0' || value[i] > '9') {
            cout << "Input must contain only digits. \n";
            return false;
        }
    }
    return true;
}

void printMenu(int length) {

    cout << endl;
    cout << "==========================================" << endl;
    cout << "        Welcome to Bulls and Cows!" << endl;
    cout << "==========================================" << endl;
    cout << "Rules: " << endl;
    cout << "1. You and the AI will each choose a " << length << " - digit secret code." << endl;
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