//
// Bulls and Cows game
// Easy AI
// By Jhervis Arevalo and Lars Timajo
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

struct GameSetting{
    int length;
    int maxGuesses = 7;
    int difficulty = 1; // 1 easy, 2 medium, 3 hard
};
struct GameState {
    string playerSecretCode;
    string aiSecretCode;
    vector<string> aiGuesses;
    int turn = 1;
};
struct AiGuessInfo {
    int bull;
    int cow;
};

void setupGame();
void startEasyGame(const GameSetting& game, GameState& state);
void startMediumGame(const GameSetting& game, GameState& state, AiGuessInfo& aiInfo);
void printMenu(const GameSetting& setting);
int countGuess(string input, string secretCode, string returnType);
string generateUniqueDigitCode(int length);
string getPlayerSecretCode(int length);
void mediumAIGuess(int length, GameState& state, vector<AiGuessInfo> info);
bool checkWinningCondition(int pBull, int aiBull, int length, int round);
bool validationDigitCode(string code, int length);
bool hasDuplicateDigits(string code);

int main()
{
    srand(time(0));
    char choice;
    do {
        setupGame();

        cout << "Play another game? (y/n): ";
        cin >> choice; cin.ignore();
    } while (tolower(choice) == 'y');
}

void setupGame() {

    GameSetting settings;
    GameState state;
    AiGuessInfo aiGuess;
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

    state.aiSecretCode = generateUniqueDigitCode(settings.length);
    state.playerSecretCode = getPlayerSecretCode(settings.length);

    switch (settings.difficulty) {
    case 1: startEasyGame(settings, state); break;
    case 2: startMediumGame(settings, state, aiGuess); break;
    }
}

void startEasyGame(const GameSetting& game, GameState& state) {

    string playerGuess, aiGuess;

    while (state.turn <= 7) {
        int bullPlayer, cowPlayer, bullAi, cowAi;
        cout << "=============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player Guess: ";
        cin >> playerGuess; cin.ignore();

        if (!validationDigitCode(playerGuess, game.length)) continue;
        if (hasDuplicateDigits(playerGuess)) continue;

        aiGuess = generateUniqueDigitCode(game.length);
        cout << "AI Guess: " << aiGuess;

        bullPlayer = countGuess(playerGuess, state.aiSecretCode, "bull");
        cowPlayer = countGuess(playerGuess, state.aiSecretCode, "cow");
        bullAi = countGuess(aiGuess, state.playerSecretCode, "bull");
        cowAi = countGuess(aiGuess, state.playerSecretCode, "cow");

        cout << "\nPlayer's Bull: " << bullPlayer << endl;
        cout << "Player's Cow: " << cowPlayer << endl;
        cout << "Ai's Bull: " << bullAi << endl;
        cout << "Ai's Cow: " << cowAi << endl;

        if(checkWinningCondition(bullPlayer, bullAi, game.length, state.turn)) break;
        state.turn++;
    }
    cout << "Player's Secret Code: " << state.playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;
}

void startMediumGame(const GameSetting& game, GameState& state, AiGuessInfo& aiInfo) {

    string playerGuess, aiGuess;
    vector<AiGuessInfo> aiHistory;

    while (state.turn <= 7) {
        int bullPlayer, cowPlayer, bullAi, cowAi;
        cout << "=============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player Guess: ";
        cin >> playerGuess; cin.ignore();

        if (!validationDigitCode(playerGuess, game.length)) continue;
        if (hasDuplicateDigits(playerGuess)) continue;

        mediumAIGuess(game.length, state, aiHistory);
        
        bullPlayer = countGuess(playerGuess, state.aiSecretCode, "bull");
        cowPlayer = countGuess(playerGuess, state.aiSecretCode, "cow");
        bullAi = countGuess(aiGuess, state.playerSecretCode, "bull");
        cowAi = countGuess(aiGuess, state.playerSecretCode, "cow");

        AiGuessInfo aiGuess = { bullAi, cowAi };
        aiHistory.push_back(aiGuess);

        cout << "\nPlayer's Bull: " << bullPlayer << endl;
        cout << "Player's Cow: " << cowPlayer << endl;
        cout << "Ai's Bull: " << bullAi << endl;
        cout << "Ai's Cow: " << cowAi << endl;

        if (checkWinningCondition(bullPlayer, bullAi, game.length, state.turn)) break;
    }
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

void mediumAIGuess(int length, GameState& state, vector<AiGuessInfo> info) {

    int turn = state.turn - 1;
    if (turn == 0) {
        state.aiSecretCode = generateUniqueDigitCode(length);
        state.aiGuesses.push_back(state.aiSecretCode);
        return;
    }

    while (state.aiGuesses[turn].length() < length) {

        if (info[turn].bull == 0 && info[turn].cow == 0) {
            state.aiSecretCode = generateUniqueDigitCode(length);
            state.aiGuesses.push_back(state.aiSecretCode);
            return;
        }
        if (info[turn].bull > 0 && info[turn].bull < 4) {

        }
    }
}

string generateUniqueDigitCode(int length) {

    string RandomCode = "";

    while (RandomCode.length() < length) {
        char random = (rand() % 10) + '0';

        if (RandomCode.find(random) == string::npos) {
            RandomCode += random;
        }
    }
    return RandomCode;
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