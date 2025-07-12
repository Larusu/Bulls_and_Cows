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

struct GameSetting{
    int length;
    int maxGuesses = 7;
    int difficulty = 1; // 1 easy, 2 medium, 3 hard
};

struct GameState {
    string playerSecretCode;
    string aiSecretCode;
    string lastPlayerCode;
    string lastAiCode;
    int turn = 1;
};

void setupGame(GameSetting& setting, GameState& state);
void startEasyGame(const GameSetting& game, GameState& state);
void startMediumGame(const GameSetting& game, GameState& state);
void printMenu(const GameSetting& setting);
int countGuess(string input, string secretCode, string returnType);
string easyAIGuess(int length);
string generateAiSecretCode(int length);
string getPlayerSecretCode(int length);
bool checkWinningCondition(int pBull, int aiBull, int length, int round);
bool validationDigitCode(string code, int length);
bool hasDuplicateDigits(string code);

int main()
{
    srand(time(0));

    GameSetting settings;
    GameState state;
    char choice;
    do {
        setupGame(settings, state);

        cout << "Play another game? (y/n): ";
        cin >> choice; cin.ignore();
    } while (tolower(choice) == 'y');
}

void setupGame(GameSetting& settings, GameState& state) {

    printMenu(settings);

    while (settings.length == 0) {
        cout << "Choose secret code length: ";
        cin >> settings.length;
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Must be a number. \n";
            settings.length = 0;
            continue;
        }
        cin.ignore();

        if (settings.length > 9 || settings.length < 4) {
            cout << "Invalid input! Length must only be 4 - 9." << endl;
            settings.length = 0;
        }
    }

    while (settings.difficulty != 1 && settings.difficulty != 2) {
        cout << "Choose the difficulty of the game (1 for easy, 2 for hard): ";
        cin >> settings.difficulty; 
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Choose only 1 (Easy) or 2 (Medium). \n";
            continue;
        }
        cin.ignore();

        if (settings.difficulty < 1 || settings.difficulty > 2) {
            cout << "Choose only 1 or 2";
        }
    }

    state.aiSecretCode = generateAiSecretCode(settings.length);
    state.playerSecretCode = getPlayerSecretCode(settings.length);

    switch (settings.difficulty) {
    case 1: startEasyGame(settings, state); break;
    case 2: startMediumGame(settings, state); break;
    }
}

void startEasyGame(const GameSetting& game, GameState& state) {

    string playerGuess, aiGuess;
    int count = 1;

    while (count <= 7) {
        int bullPlayer, cowPlayer, bullAi, cowAi;
        cout << "=============================" << endl;
        cout << "Guess #" << count << endl;
        cout << "Player Guess: ";
        cin >> playerGuess; cin.ignore();

        if (!validationDigitCode(playerGuess, game.length)) continue;
        if (hasDuplicateDigits(playerGuess)) continue;

        aiGuess = easyAIGuess(game.length);
        cout << "AI Guess: " << aiGuess;

        bullPlayer = countGuess(playerGuess, state.aiSecretCode, "bull");
        cowPlayer = countGuess(playerGuess, state.aiSecretCode, "cow");
        bullAi = countGuess(aiGuess, state.playerSecretCode, "bull");
        cowAi = countGuess(aiGuess, state.playerSecretCode, "cow");

        cout << "\nPlayer's Bull: " << bullPlayer << endl;
        cout << "Player's Cow: " << cowPlayer << endl;
        cout << "Ai's Bull: " << bullAi << endl;
        cout << "Ai's Cow: " << cowAi << endl;

        if(checkWinningCondition(bullPlayer, bullAi, game.length, count)) break;
        count++;
    }
    cout << "Player's Secret Code: " << state.playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;

    state.lastPlayerCode = state.playerSecretCode;
    state.lastAiCode = state.aiSecretCode;
}

void startMediumGame(const GameSetting& game, const GameState& state) {

}

void printMenu(const GameSetting& game) {

    cout << endl;
    cout << "==========================================" << endl;
    cout << "        Welcome to Bulls and Cows!" << endl;
    cout << "==========================================" << endl;
    cout << "Rules: " << endl;
    cout << "1. You will choose the length of the game's secret code. It can only be from 4 - 9." << endl;
    cout << "2. You need to choose the difficulty of the game (Easy, Medium). " << endl;
    cout << "3. You and the AI will each guess each other secret code." << endl;
    cout << "4. Each digit must be unique and between 0 and 9." << endl;
    cout << "5. You will try to guess the AI's code, and the AI will try to guess yours." << endl;
    cout << "6. After each guess, you'll be told how many Bulls and Cows you got:" << endl;
    cout << "   - Bulls: Correct digit in the correct position." << endl;
    cout << "   - Cows:  Correct digit in the wrong position." << endl;
    cout << "7. Each side has a maximum of " << game.maxGuesses <<" guesses." << endl;
    cout << "   - If neither side guesses the correct answer within " << game.maxGuesses << " attempts," << endl;
    cout << "     the game ends in draw." << endl;
    cout << "Let's start!" << endl << endl;
}

int countGuess(string input, string secretCode, string returnType) {

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

string easyAIGuess(int length) {

    string guessCode = "";
    while (guessCode.length() < length) {
        char random = (rand() % 10) + '0';

        if (guessCode.find(random) == string::npos) {
            guessCode += random;
        }
    }
    return guessCode;
}

string generateAiSecretCode(int length) {

    string AISecretCode = "";

    while (AISecretCode.length() < length) {
        char random = (rand() % 10) + '0';

        if (AISecretCode.find(random) == string::npos) {
            AISecretCode += random;
        }
    }
    return AISecretCode;
}

string getPlayerSecretCode(int length) {

    string playerSecretCode;

    while (playerSecretCode.empty()) {
        cout << "Enter your secret code (" << length << " unique digits): ";
        cin >> playerSecretCode; cin.ignore();

        if (!validationDigitCode(playerSecretCode, length)) continue;

        if (hasDuplicateDigits(playerSecretCode)) continue;
    }
    return playerSecretCode;
}

bool checkWinningCondition(int pBull, int aiBull, int length, int round) {

    if (pBull == length) {
        cout << "\nCongratulations! You won!" << endl;
        return true;
    }
    else if (pBull == length) {
        cout << "\nCongratulations! You won!" << endl;
        return true;
    }
    else if (round == length) {
        return true;
    }
    return false;
}

bool validationDigitCode(string code, int length) {

    if (code.length() != length) {
        cout << "Invalid length. Try again.\n";
        return false;
    }
    for (int i = 0; i < code.length(); i++) {
        if (code[i] < '0' || code[i] > '9') {
            cout << "Input must contain only digits. \n";
            return false;
        }
    }
    return true;
}

bool hasDuplicateDigits(string code) {

    for (int i = 0; i < code.length(); i++) {
        for (int j = 0; j < i; j++) {
            if (code[i] == code[j]) {
                cout << "Digits must be unique.\n"; 
                return true;
            }
        }
    }
    return false;
}