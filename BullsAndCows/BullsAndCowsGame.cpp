#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <algorithm>

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

// For tracking each guess along with its bulls and cows count, only for Hard AI
struct GuessInfo
{
    string guess = "";
    int bull = 0;
    int cow = 0;
};

// Game Control Flow
void setupGame(GameSetting& settings, GameState& state);                // Sets up game setting and secret code
void startGame(const GameSetting& setting, GameState& state);           // Starts the game loop
void printMenu(const GameSetting& game);                                // Displays main menu

// Ai's
string generateEasyAiCode(int length);                                               // Generates random code every turn
string generateMediumAiCode(const vector<GuessInfo>& history, int turn, int length);  // Generates non-repeating code
string generateHardAiCode(vector<GuessInfo>& history, int turn, int length);          // Generates smart guesses based on history

// Game Mechanics
pair<int, int> getResultCowAndBull(const string& secret, const string& guess, int length);       // Returns bulls and cows as a pair
bool checkWinningCondition(const GameSetting& setting, GameState& state, int pBull, int aiBull); // Checks if someone won or if it's a draw

// Utility function
string generateUniqueDigitCode(int length);                             // Generates a unique-digit random code
string getPlayerSecretCode(int length);                                 // Prompts player's secret code
bool isValidDigitCode(string code, int length);                         // Checks for input's correct length and digit
bool hasDuplicateDigits(const string& code);                            // Checks for duplicate digits, only for 0 - 9
bool hasDuplicateDigits(const string& code, const string& compare);     // Comparing if 2 strings has duplicate


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
        state.turn = 1; // Making sure that every game turn starts at 1
        setupGame(settings, state); // Set up and play the game

        while (true)
        {
            cout << "Play another game? (y/n): ";
            cin >> choice; cin.ignore();
            if (tolower(choice) != 'y' && tolower(choice) != 'n')
            {
                cout << "Invalid input. Choose only Y, y, N, or n. \n";
                continue;
            }
            break;
        }
    } while (tolower(choice) == 'y');
}

// Changing game settings here
void setupGame(GameSetting& gameSettings, GameState& gameState)
{
    while (true)
    {
        cout << "Choose the difficulty of the game (1 for Easy, 2 for Medium, 3 for Hard): ";
        cin >> gameSettings.difficulty;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Choose only 1 (Easy), 2 (Medium), or 3 (Hard). \n";
            continue;
        }
        cin.ignore();

        if (gameSettings.difficulty < 1 || gameSettings.difficulty > 3)
        {
            cout << "Choose only 1, 2, or 3. \n";
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
    vector<GuessInfo>aiPastInformation; // for medium and hard AI: stores all the past guess information (guess, bulls, cows)

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
                aiGuess = generateMediumAiCode(aiPastInformation, state.turn, setting.maxCodeLength);
                break;
            case 3:
                aiGuess = generateHardAiCode(aiPastInformation, state.turn, setting.maxCodeLength);
                break;
        }
        cout << "AI Guess: " << aiGuess << endl;

        // Calculating number of bulls and cows. Access with .first (bulls), .second (cows)        
        auto playerResult = getResultCowAndBull(state.aiSecretCode, pGuess, setting.maxCodeLength);
        auto aiResult = getResultCowAndBull(state.playerSecretCode, aiGuess, setting.maxCodeLength);

        // For medium and hard AI: Storing information into aiPastInformation
        if (setting.difficulty == 2 || setting.difficulty == 3)
        {
            GuessInfo temp = { aiGuess, aiResult.first, aiResult.second };
            aiPastInformation.push_back(temp);
        }

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

// Generates a unique random code based on past guesses
string generateMediumAiCode(const vector<GuessInfo>& history, int turn,  int length)
{
    string RandomCode(length, '\0'); // initialize with null characters
    int round = turn - 1;

    // For the first 3 turn, go random for testing data
    if (round <= 2)
    {
        while (round == 1)
        {
            string firstTurn = history[0].guess;
            RandomCode = generateUniqueDigitCode(length);

            if (!hasDuplicateDigits(RandomCode, firstTurn))
            {
                return RandomCode;
            }
        }
        return generateUniqueDigitCode(length);
    }

    vector<char> digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    vector<char> possibleDigits;

    // Step 1: eliminating digits that are definitely not in player secret code
    for (int i = 0; i < round; i++)
    {
        if (history[i].bull + history[i].cow == 0)
        {
            for (char c : history[i].guess)
            {
                digits.erase(remove(digits.begin(), digits.end(), c), digits.end());
            }
        }
    }

    // Step 2: Locking bull positions based on repeated bull position
    for (int i = 0; i < round - 1; i++)
    {
        for (int j = 0; j < length; j++)
        {
            char currentGuessChar = history[round - 1].guess[j];

            // If both current and previous guesses have bulls and the digit matches
            if (history[round - 1].bull > 0 &&
                history[i].bull > 0 &&
                history[i].guess[j] == currentGuessChar)
            {
                RandomCode[j] = currentGuessChar;
            }
        }
    }

    // Step 3: Collect possible cow digits from all past guesses
    for (int i = 0; i < round; i++)
    {
        if (history[i].cow + history[i].bull > 0)
        {
            for (char c : history[i].guess)
            {
                possibleDigits.push_back(c);
            }
        }
    }

    // Remove duplicates from possibleDigits
    sort(possibleDigits.begin(), possibleDigits.end());
    possibleDigits.erase(unique(possibleDigits.begin(), possibleDigits.end()), possibleDigits.end());

    // Step 4: Fill remaining empty spots
    for (int i = 0; i < length; i++)
    {
        if (RandomCode[i] != '\0') continue;

        int random;
        // Step 4.1: Choosing from cow candidates first
        if (!possibleDigits.empty())
        {
            char candidate = possibleDigits[rand() % possibleDigits.size()];

            if (RandomCode.find(candidate) == string::npos)
            {
                RandomCode[i] = candidate;
                digits.erase(remove(digits.begin(), digits.end(), candidate), digits.end()); // preventing duplicates
                continue;
            }
        }

        // Step 4.2: Fill remaining digits
        while (true)
        {
            char random = digits[rand() % digits.size()];

            if (RandomCode.find(random) == string::npos)
            {
                RandomCode[i] = random;
                digits.erase(remove(digits.begin(), digits.end(), random), digits.end()); // preventing duplicates
                break;
            }
        }
    }
    return RandomCode; // Step 5: return final generated AI guess
}

// Generates a unique random code based on past guesses
string generateHardAiCode(vector<GuessInfo>& history, int turn, int length)
{
    
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

bool hasDuplicateDigits(const string& code)
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

bool hasDuplicateDigits(const string& code, const string& compare)
{
    for (char c : code)
    {
        if (compare.find(c) != string::npos)
        {
            return true;
        }
    }
    return false;
}