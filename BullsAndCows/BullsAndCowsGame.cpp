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

void setupGame(GameSetting& settings, GameState& state)
{ // Changing game settings here
    
    
    while (settings.difficulty != 1 && settings.difficulty != 2)
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
    }

    // Generating Secret Code
    state.aiSecretCode = generateUniqueDigitCode(settings.length);
    state.playerSecretCode = getPlayerSecretCode(settings.length);

    switch (settings.difficulty)
    {
    case 1: startEasyGame(settings, state); break;
    case 2: startMediumGame(settings, state); break;
    }
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