#include <iostream>  
#include "secret_code.h"  

using namespace std;  

const int MAX_SIZE = 4;  

int main() {  

	int* aiSecretCode = generateAiSecretCode(MAX_SIZE); 
	if (!aiSecretCode) {  
		cerr << "Error: Failed to generate AI secret code." << endl;  
		return 1;  
	}  

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