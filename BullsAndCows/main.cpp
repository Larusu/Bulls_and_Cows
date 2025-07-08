#include <iostream>  
#include "secret_code.h"  

using namespace std;  

int main()
{
	int codeLength;

	printMenu();

	cout << "\nSecret code length: ";
	cin >> codeLength;

	string AISecretCode = generateAiSecretCode(codeLength);
	string playerSecretCode = getPlayerSecretCode(codeLength);

	cout << "\nAI Secret Code: " << AISecretCode << endl;
	cout << "Player Secret Code: " << playerSecretCode;

	return 0;
}