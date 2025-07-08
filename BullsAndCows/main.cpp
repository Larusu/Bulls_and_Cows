#include <iostream>  
#include "secret_code.h"  

using namespace std;  

const int MAX_SIZE = 4;  

int main() {  

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
	
	int* aiSecretCode = generateAiSecretCode(MAX_SIZE); 
	if (!aiSecretCode) {  
		cerr << "Error: Failed to generate AI secret code." << endl;  
		return 1;  
	}  cout << "AI code: ####" << endl;

	int* playerSecretCode = getPlayerSecretCode(MAX_SIZE);  
	if (!playerSecretCode) {  
		cerr << "Error: Failed to get player secret code." << endl;  
		delete[] aiSecretCode;  
		return 1;  
	}  

	for (int i = 0; i < MAX_SIZE; i++) {  
		cout << playerSecretCode[i] << " ";  
	} cout << endl;  

	for (int i = 0; i < MAX_SIZE; i++) {  
		cout << aiSecretCode[i] << " ";  
	} cout << endl;  

	delete[] playerSecretCode;  
	delete[] aiSecretCode;  

	return 0;  
}