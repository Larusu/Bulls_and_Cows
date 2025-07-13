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

struct GameSetting
{
    int length;
    int maxGuesses = 7;
    int difficulty = 1; // 1 easy, 2 medium, 3 hard
};
struct GameState 
{ // Tracking the progress and storing each secret code
    string playerSecretCode;
    string aiSecretCode;
    int turn = 1;
};
struct GuessInfo 
{ // Storing Player and Ai bulls and cows
    string guess = "";
    int bull;
    int cow;
};

void setupGame();
void startEasyGame(const GameSetting& game, GameState& state);
void startMediumGame(const GameSetting& game, GameState& state);
void printMenu(const GameSetting& setting);
string generateUniqueDigitCode(int length);
string getPlayerSecretCode(int length);
void mediumAIGuess(int length, GuessInfo& Ai, GameState& state, vector<GuessInfo>& info);
void getBullsAndCowsForTurn(const GameState& state, GuessInfo& player, GuessInfo& ai, int length);
bool checkWinningCondition(int pBull, int aiBull, int length, int round);
bool validationDigitCode(string code, int length);
bool hasDuplicateDigits(string code);

int main()
{
    srand(time(0));
    char choice;
    do 
    {
        setupGame();
        cout << "Play another game? (y/n): ";
        cin >> choice; cin.ignore();
    } 
    while (tolower(choice) == 'y');
}

void setupGame()
{ // Changing game settings here
    GameSetting settings;
    GameState state;
    printMenu(settings);

    while (settings.length == 0) 
    {
        cout << "Choose secret code length: ";
        cin >> settings.length;
        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Must be a number. \n";
            settings.length = 0;
            continue;
        }
        cin.ignore();

        if (settings.length > 9 || settings.length < 4) 
        {
            cout << "Invalid input! Length must only be 4 - 9." << endl;
            settings.length = 0;
        }
    }

    while (settings.difficulty != 1 && settings.difficulty != 2) 
    {
        cout << "Choose the difficulty of the game (1 for easy, 2 for hard): ";
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

// The easy difficulty game itself
void startEasyGame(const GameSetting& game, GameState& state) 
{
    GuessInfo Player;
    GuessInfo Ai;
    int length = game.length;

    while (state.turn <= 7) 
    {
        cout << "=============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player Guess: ";
        cin >> Player.guess; cin.ignore();

        // Validations for player input
        if (!validationDigitCode(Player.guess, length)) continue;
        if (hasDuplicateDigits(Player.guess)) continue;

        Ai.guess = generateUniqueDigitCode(length); // Random guess
        cout << "AI Guess: " << Ai.guess;

        // Storing data of 'bulls' And 'cows' on Player and Ai
        getBullsAndCowsForTurn(state, Player, Ai, length); 

        cout << "\nPlayer's Bull: " << Player.bull << endl;
        cout << "Player's Cow: " << Player.cow << endl;
        cout << "Ai's Bull: " << Ai.bull << endl;
        cout << "Ai's Cow: " << Ai.cow << endl;

        if(checkWinningCondition(Player.bull, Ai.bull, length, state.turn)) break;

        state.turn++;
    }
    cout << "Player's Secret Code: " << state.playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;
}

// The medium difficulty game itself
void startMediumGame(const GameSetting& game, GameState& state) 
{
    string playerGuess, aiGuess;
    GuessInfo Player;
    GuessInfo Ai;
    vector<GuessInfo> aiHistory; // to store the bulls and cows per guess

    while (state.turn <= 7) 
    {
        cout << "=============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player Guess: ";
        cin >> playerGuess; cin.ignore();
        // Validations for player input
        if (!validationDigitCode(playerGuess, game.length)) continue;
        if (hasDuplicateDigits(playerGuess)) continue;

        // Storing Ai guess on vector state.aiGuesses
        mediumAIGuess(game.length, Ai, state, aiHistory); 
        
        GuessInfo aiGuessResult = { Ai.guess, Ai.bull, Ai.cow};
        aiHistory.push_back(aiGuessResult);

        // Storing data of 'bulls' And 'cows' on Player and Ai
        getBullsAndCowsForTurn(state, Player, Ai, game.length);

        cout << "\nPlayer's Bull: " << Player.bull << endl;
        cout << "Player's Cow: " << Player.cow << endl;
        cout << "Ai's Bull: " << Ai.bull << endl;
        cout << "Ai's Cow: " << Ai.cow << endl;

        if (checkWinningCondition(Player.bull, Ai.bull, game.length, state.turn)) break;

        state.turn++;
    }
    cout << "Player's Secret Code: " << state.playerSecretCode << endl;
    cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;
}

void printMenu(const GameSetting& game) 
{
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

void getBullsAndCowsForTurn(const GameState& state, GuessInfo& player, GuessInfo& ai, int length)
{
    for (int i = 0; i < length; i++) 
    {
        if (i == state.aiSecretCode[i] && state.aiSecretCode[i] == player.guess[i])
        { // checkign for player's bull
            player.bull++; continue;
        }
        for (int j = 0; j < length; j++) 
        { // checking for player's cow
            if (player.guess[i] == state.aiSecretCode[j]) player.cow++; break; 
        }
    }

    for (int i = 0; i < length; i++) 
    { 
        if (i == state.playerSecretCode[i] && state.playerSecretCode[i] == ai.guess[i])
        { // checking for ai's bull
            ai.bull++;  continue;
        }
        for (int j = 0; j < length; j++)
        { // checking for ai's cow
            if (ai.guess[i] == state.playerSecretCode[j]) ai.cow++; break;
        }
    }
}

void mediumAIGuess(int length, GuessInfo& Ai, GameState& state, vector<GuessInfo>& info) 
{
    vector<char> digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    
    int turn = state.turn - 1;
    char newCode[4] = { '\0', '\0', '\0', '\0' };

    // if first turn or last guess had 0 bulls and cows
    if (turn == 0 || (info[turn].bull + info[turn].cow == 0))
    {
        Ai.guess = generateUniqueDigitCode(length); // Storing new guess into Ai.guess
        return;
    }

    // removing 'digits' elements that were confirmed as wrong
    for (int i = 0; i <= turn; i++)
    {
        if (info[i].bull + info[i].cow == 0) 
        {
            for (char c : info[i].guess)
            {
                digits.erase(remove(digits.begin(), digits.end(), c), digits.end());
            }
        }
    }

    // For generating new guess
    for(int i = 0; i < length; i++)
    {
        if (newCode[i] != '\0') continue;

        int random = rand() % digits.size();
        char digit = digits[random];

        bool duplicate = false;
        for (int j = 0; j < i; j++) 
        {
            if (newCode[j] == digit) 
            {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) 
        {
            newCode[i] = digit;
            continue;
        }
        i--;
    }
    for (int i = 0; i < length; i++) 
    {
        Ai.guess += newCode[i]; // Storing new guess into Ai.guess
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

bool checkWinningCondition(int pBull, int aiBull, int length, int round) 
{
    if (pBull == length) 
    {
        cout << "\nCongratulations! You won!" << endl;
        return true;
    }
    else if (pBull == length) 
    {
        cout << "\nCongratulations! You won!" << endl;
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