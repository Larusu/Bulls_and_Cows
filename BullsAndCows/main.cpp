#include <iostream>  
#include <fstream>
#include <string>
#include "secret_code.h"  

using namespace std;  

int main()
{
	startGame();

	fstream aiSecretCodeFile("AiSecretCode.txt", ios::in);
	fstream playerSecretCodeFile("PlayerSecretCode.txt", ios::in);

	if (!aiSecretCodeFile.is_open() || !playerSecretCodeFile.is_open()) {
		cerr << "Error opening files." << endl;
		return 1;
	}

	string aiSecretCode;
	string playerSecretCode;
	getline(aiSecretCodeFile, aiSecretCode);
	getline(playerSecretCodeFile, playerSecretCode);

	aiSecretCodeFile.close();
	playerSecretCodeFile.close();

	cout << "\nAI Secret Code: " << aiSecretCode << endl;
	cout << "Player Secret Code: " << playerSecretCode;

	return 0;
}