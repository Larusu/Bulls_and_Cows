#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>

using namespace std;

// For tweaking game settings
struct GameSetting
{
    const int maxCodeLength = 4; // Adjust this value for harder difficulty
    int maxGuesses = 7;
    int difficulty = 1; // 1 = easy, 2 = medium, 3 = hard
};

// For tracking progress and storing each secret code
struct GameState
{
    string playerSecretCode;
    string aiSecretCode;
    int turn = 1;
    int aiScore = 0;
    int playerScore = 0;
};

// Game Control Flow
void setupGame(GameSetting& settings, GameState& state);                // Sets up game setting and secret code
void startGame(const GameSetting& setting, GameState& state);           // Starts the game loop
void printMenu(const GameSetting& game);                                // Displays main menu

// Ai's
string generateEasyAiCode(int length);                                  // Generates random code every turn
string generateMediumAiCode(GameState& state, vector<string>& history, int length);  // Generates non-repeating code

// Game Mechanics
pair<int, int> getResultCowAndBull(const string& secret, const string& guess, int length); // Returns bulls and cows as a pair
bool checkWinningCondition(const GameSetting& setting, GameState& state, int pBull, int aiBull); // Checks if someone won or if it's a draw

// Utility function
string generateUniqueDigitCode(int length);                             // Generates a unique-digit random code
string getPlayerSecretCode(int length);                                 // Prompts player's secret code
bool isValidDigitCode(string code, int length);                         // Checks for input's correct length and digit
bool hasDuplicateDigits(string code);                                   // Checks for duplicate digits


int main()
{
    srand(time(0));

    // Initializing game setting  and state
    GameSetting settings;
    GameState state;
    char choice;

    printMenu(settings); // Display the game rules
    do
    {
        setupGame(settings, state); // Set up and play the game
        cout << "Play another game? (y/n): ";
        cin >> choice; cin.ignore();
    } while (tolower(choice) == 'y');
}

// Changing game settings here
void setupGame(GameSetting& gameSettings, GameState& gameState)
{
    while (true)
    {
        cout << "Choose the difficulty of the game (1 for Easy, 2 for Medium): ";
        cin >> gameSettings.difficulty;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Choose only 1 (Easy) or 2 (Medium). \n";
            continue;
        }
        cin.ignore();

        if (gameSettings.difficulty < 1 || gameSettings.difficulty > 2)
        {
            cout << "Choose only 1 or 2. \n";
            continue;
        }
        break;
    }

    // Generating Secret Code
    gameState.aiSecretCode = generateUniqueDigitCode(gameSettings.maxCodeLength);
    gameState.playerSecretCode = getPlayerSecretCode(gameSettings.maxCodeLength);

    startGame(gameSettings, gameState);
}

// Where the game starts
void startGame(const GameSetting& setting, GameState& state)
{
    string pGuess, aiGuess;
    vector<string>aiHistoryGuess; // for medium AI, stores past guesses in a vector

    while (state.turn <= setting.maxGuesses)
    {
        cout << "=============================" << endl;
        cout << "Guess #" << state.turn << endl;
        cout << "Player Guess: ";
        cin >> pGuess;
        cin.ignore();

        // Validate input length and uniqueness
        if (!isValidDigitCode(pGuess, setting.maxCodeLength)) continue;
        if (hasDuplicateDigits(pGuess)) continue;

        // Checking difficulty: 1 = easy, 2 = medium
        switch (setting.difficulty)
        {
            case 1:
                aiGuess = generateEasyAiCode(setting.maxCodeLength);
                break;
            case 2:
                aiGuess = generateMediumAiCode(state, aiHistoryGuess, setting.maxCodeLength);
                aiHistoryGuess.push_back(aiGuess);
                break;
        }
        cout << "AI Guess: " << aiGuess << endl;

        // Calculating number of bulls and cows. Access with .first (bulls), .second (cows)        
        auto playerResult = getResultCowAndBull(state.aiSecretCode, pGuess, setting.maxCodeLength);
        auto aiResult = getResultCowAndBull(state.playerSecretCode, aiGuess, setting.maxCodeLength);

        // Printing results
        cout << "\nPlayer's Bull: " << playerResult.first << endl;
        cout << "Player's Cow: " << playerResult.second << endl;
        cout << "Ai's Bull: " << aiResult.first << endl;
        cout << "Ai's Cow: " << aiResult.second << endl;

        // If player or ai wins, or if the game is draw
        if (checkWinningCondition(setting, state, playerResult.first, aiResult.first))
        {
            cout << "Player's Secret Code: " << state.playerSecretCode << endl;
            cout << "Ai's Secret Code:     " << state.aiSecretCode << endl;
            cout << "\nScore: " << endl;
            cout << "Player's score: " << state.playerScore << endl;
            cout << "Ai's score: " << state.aiScore << endl;
            cout << "==============================" << endl;
            break;
        }

        state.turn++; // move to next turn if no one won
    }
}

// Displaying main menu
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

// Generates a new random code each turn (possible but rarely repeats)
string generateEasyAiCode(int length)
{
    return generateUniqueDigitCode(length);
}

// Generates a unique random code that has not been guess by the AI before
string generateMediumAiCode(GameState& state, vector<string>& history, int length)
{
    string RandomCode = generateUniqueDigitCode(length);
    int turn = state.turn - 1;

    // If first turn, generates a unique code
    if (turn == 0)
    {
        return RandomCode;
    }

    int index = 0;
    while (index < turn) // Rejecting exact matches from the past guess
    {
        string compareTrash = history[index];

        if (!(compareTrash[0] != RandomCode[0] ||
            compareTrash[1] != RandomCode[1] ||
            compareTrash[2] != RandomCode[2] ||
            compareTrash[3] != RandomCode[3]))
        {
            RandomCode = generateUniqueDigitCode(length);

            // For safety, checking if first turn
            if (turn == 0)
            {
                index = 0;
                continue;
            }

            index--;
            continue;
        }
        index++;
    }
    return RandomCode;
}

// Returning a pair of integers: (bulls, cows)
pair<int, int> getResultCowAndBull(const string& secret, const string& guess, int length)
{
    int cow = 0, bull = 0;

    for (int i = 0; i < length; i++)
    {
        if (secret[i] == guess[i]) // bull : correct digit and position
        {
            bull++;
            continue;
        }

        for (int j = 0; j < length; j++)
        {
            if (guess[i] == secret[j] && i != j) // cow : correct digit but wrong position
            {
                cow++;
                break;
            }
        }
    }
    return { bull, cow };
}

// Returns true if the player or AI wins, or if the game ends in a draw
bool checkWinningCondition(const GameSetting& setting, GameState& state, int pBull, int aiBull)
{
    if (pBull >= setting.maxCodeLength) // Player wins: Player's bull count matches code length
    {
        state.playerScore++;
        cout << "==============================" << endl;
        cout << "Congratulations! You won!" << endl;
        return true;
    }
    else if (aiBull >= setting.maxCodeLength) // AI wins: AI's bull count matches code length
    {
        state.aiScore++;
        cout << "==============================" << endl;
        cout << "You lost. Better luck next time!" << endl;
        return true;
    }
    else if (state.turn >= setting.maxGuesses) // Draw: max number of turns reached
    {
        cout << "==============================" << endl;
        cout << "It's a draw! No one wins :(" << endl;
        return true;
    }

    return false; // game continues
}

// Generates a random code with unique digits
string generateUniqueDigitCode(int length)
{
    string RandomCode = "";

    while (RandomCode.length() < length)
    {
        char random = (rand() % 10) + '0'; // random digit from 0 to 9

        // Ensuring digit has haven't used
        if (RandomCode.find(random) == string::npos)
        {
            RandomCode += random;
        }
    }
    return RandomCode;
}

// Getting a valid secret code from player
string getPlayerSecretCode(int length)
{
    string playerSecretCode;

    while (playerSecretCode.empty())
    {
        cout << "Enter your secret code (" << length << " unique digits): ";
        cin >> playerSecretCode; cin.ignore();

        // Validate input length and uniqueness
        if (!isValidDigitCode(playerSecretCode, length)) continue;
        if (hasDuplicateDigits(playerSecretCode)) continue;
    }
    return playerSecretCode;
}

bool isValidDigitCode(string code, int length)
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