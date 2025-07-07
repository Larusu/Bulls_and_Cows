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
	int index = 0;

	while (index < size) {
		cout << "Your code: ";
		cin >> input;

		index++;
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