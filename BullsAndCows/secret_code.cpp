#include <iostream>
#include <cstdlib> 
#include <ctime>
#include "secret_code.h"

using namespace std;

int* generateAiSecretCode(int size) {

	srand(time(0));
	int* number = new int[size];
	int index = 0;
	while (index < size) {

		int random = rand() % 10;

		if (checkSameValue(random, number, index)) {
			continue;
		}
		number[index] = random;
		index++;
	}
	return number;
}

int* getPlayerSecretCode(int size) {  

	int* secretCode = new int[size];  
	int input;  

	while (true) {  
		cout << "Your code: ";  
		cin >> input;  

		if (input < 1000 || input > 9999) {  
			cout << "Please enter a 4-digit number." << endl;  
			continue;  
		}  

		for (int i = size - 1; i >= 0; i--) {  
			secretCode[i] = input % 10;  
			input /= 10;  
		}  

		bool duplicate = false;
		for (int i = 0; i < size; i++) {
			if (checkSameValue(secretCode[i], secretCode, i)) {
				duplicate = true;
				break;
			}
		}

		if (duplicate) {  
			cout << "Please enter a code with unique digits." << endl;  
			continue;  
		}  
		break;  
	}  
	return secretCode;  
}

bool checkSameValue(int value, int arr[], int index) {

	for (int i = 0; i < index; i++) {
		if (value == arr[i]) {
			return true;
		}
	}
	return false;
}