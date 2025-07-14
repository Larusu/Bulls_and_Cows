#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

struct GameSetting
{
    int length = 4;
    int maxGuesses = 7;
    int difficulty = 1; // 1 easy, 2 medium, 3 hard
};

struct GameState
{ // Tracking the progress and storing each secret code
    string playerSecretCode;
    string aiSecretCode;
    int turn = 1;
};

void setupGame(GameSetting& settings, GameState& state);
void printMenu(const GameSetting& game);
string generateUniqueDigitCode(int length);
string getPlayerSecretCode(int length);
pair<int, int> getResultCowAndBull(const GameState& state, string guess, int length);
bool checkWinningCondition(int pBull, int aiBull, int length, int round);
bool validationDigitCode(string code, int length);
bool hasDuplicateDigits(string code);

int main()
{
    srand(time(0));

    GameSetting settings;
    GameState state;
    char choice;

    printMenu(settings);
    do
    {
        setupGame(settings, state);
        cout << "Play another game? (y/n): ";
        cin >> choice; cin.ignore();
    } while (tolower(choice) == 'y');
}

// Changing game settings here
void setupGame(GameSetting& settings, GameState& state)
{
    while (true)
    {
        cout << "Choose the difficulty of the game (1 for easy, 2 for medium): ";
        cin >> settings.difficulty;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Choose only 1 (Easy) or 2 (Medium). \n";
            continue;
        }
        cin.ignore();

        if (settings.difficulty < 1 || settings.difficulty > 2)
        {
            cout << "Choose only 1 or 2";
        }
        break;
    }

    // Generating Secret Code
    state.aiSecretCode = generateUniqueDigitCode(settings.length);
    state.playerSecretCode = getPlayerSecretCode(settings.length);

    switch (settings.difficulty)
    {
    case 1: cout << "Test easy!" << endl; break;
    case 2: startMediumGame(settings, state); break;
    /*case 1: startEasyGame(settings, state); break;
    case 2: startMediumGame(settings, state); break;*/
    }
}

void startEasyGame(const GameSetting& setting, GameState& state)
{
    string pGuess, aiGuess;

    while (state.turn <= 7)
    {
        cout << "=============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player Guess: ";
        cin >> pGuess; cin.ignore();

        // Validations for player input
        if (!validationDigitCode(pGuess, setting.length)) continue;
        if (hasDuplicateDigits(pGuess)) continue;

        aiGuess = generateUniqueDigitCode(setting.length); // Random guess
        cout << "AI Guess: " << aiGuess;

        // first = bull, second = cow
        auto playerResult = getResultCowAndBull(state, pGuess, setting.length);
        auto aiResult = getResultCowAndBull(state, aiGuess, setting.length);

        cout << "\nPlayer's Bull: " << playerResult.first << endl;
        cout << "Player's Cow: " << playerResult.second << endl;
        cout << "Ai's Bull: " << aiResult.first << endl;
        cout << "Ai's Cow: " << aiResult.second << endl;

        if (checkWinningCondition(playerResult.first, aiResult.first, setting.length, state.turn)) break;

        state.turn++;
    }
    cout << "Player's Secret Code: " << state.playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;
}

void startMediumGame(const GameSetting& setting, GameState& state) {
    string pGuess, aiGuess;

    while (state.turn <= setting.maxGuesses) {
        cout << "==============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player guess: ";
        cin >> pGuess; cin.ignore();

        if (!validationDigitCode(pGuess, setting.length)) continue;
        if (hasDuplicateDigits(pGuess)) continue;

        aiGuess = generateUniqueDigitCode(setting.length);
        cout << "AI guess: " << aiGuess << endl;

        auto playerResult = getResultCowAndBull(state, pGuess, setting.length);
        auto aiResult = getResultCowAndBull(state, aiGuess, setting.length);

        cout << "\nPlayer's Bull: " << playerResult.first << endl;
        cout << "Player's Cow: " << playerResult.second << endl;
        cout << "Ai's Bull: " << aiResult.first << endl;
        cout << "Ai's Cow: " << aiResult.second << endl;

        if (checkWinningCondition(playerResult.first, aiResult.first, setting.length, state.turn)) break;

        state.turn++;
    }
    if (state.turn > setting.maxGuesses) {
        cout << "\n Game ended in a draw!\n";
    }
    cout << "Player's Secret Code: " << state.playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;
}

pair<int, int> getResultCowAndBull(const GameState& state, string guess, int length)
{
    int cow = 0, bull = 0;
    for (int i = 0; i < length; i++)
    {
        if (i == state.aiSecretCode[i] && state.aiSecretCode[i] == guess[i]) // checking for bull
        { 
            bull++; continue;
        }
        for (int j = 0; j < length; j++)
        { 
            if (guess[i] == state.aiSecretCode[j]) cow++; break; // checking for cow
        }
    }
    return { bull, cow };
}

string generateUniqueDigitCode(int length)
{
    string RandomCode = "";

    while (RandomCode.length() < length)
    {
        char random = (rand() % 10) + '0';

        if (RandomCode.find(random) == string::npos)
        {
            RandomCode += random;
        }
    }
    return RandomCode;
}

string getPlayerSecretCode(int length)
{
    string playerSecretCode;

    while (playerSecretCode.empty())
    {
        cout << "Enter your secret code (" << length << " unique digits): ";
        cin >> playerSecretCode; cin.ignore();

        if (!validationDigitCode(playerSecretCode, length)) continue;

        if (hasDuplicateDigits(playerSecretCode)) continue;
    }
    return playerSecretCode;
}

void printMenu(const GameSetting& game)
{
    cout << endl;
    cout << "==========================================" << endl;
    cout << "        Welcome to Bulls and Cows!" << endl;
    cout << "==========================================" << endl;
    cout << "Rules: " << endl;
    cout << "1. You need to choose the difficulty of the game (Easy, Medium). " << endl;
    cout << "2. You and the AI will each guess each other secret code." << endl;
    cout << "3. Each digit must be unique and between 0 and 9." << endl;
    cout << "4. You will try to guess the AI's code, and the AI will try to guess yours." << endl;
    cout << "5. After each guess, you'll be told how many Bulls and Cows you got:" << endl;
    cout << "   - Bulls: Correct digit in the correct position." << endl;
    cout << "   - Cows:  Correct digit in the wrong position." << endl;
    cout << "6. Each side has a maximum of " << game.maxGuesses << " guesses." << endl;
    cout << "   - If neither side guesses the correct answer within " << game.maxGuesses << " attempts," << endl;
    cout << "     the game ends in draw." << endl;
    cout << "Let's start!" << endl << endl;
}

bool checkWinningCondition(int pBull, int aiBull, int length, int round)
{
    if (pBull == length)
    {
        cout << "\nCongratulations! You won!" << endl;
        return true;
    }
    else if (aiBull == length)
    {
        cout << "\nYou lost! Betterluck next time!" << endl;
        return true;
    }
    else if (round == length)
    {
        return true;
    }
    return false;
}

bool validationDigitCode(string code, int length)
{
    if (code.length() != length)
    {
        cout << "Invalid length. Try again.\n";
        return false;
    }
    for (int i = 0; i < code.length(); i++)
    {
        if (code[i] < '0' || code[i] > '9')
        {
            cout << "Input must contain only digits. \n";
            return false;
        }
    }
    return true;
}

bool hasDuplicateDigits(string code)
{
    for (int i = 0; i < code.length(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (code[i] == code[j])
            {
                cout << "Digits must be unique.\n";
                return true;
            }
        }
    }
    return false;
}