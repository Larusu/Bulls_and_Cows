#include <iostream>
#include <cstdlib> 
#include <ctime>

using namespace std;

int* generateSecretAiCode(int size);
bool checkSameValue(int value, int arr[], int index);

int main() {

	int* array = generateSecretAiCode(4);

	for (int i = 0; i < 4; i++) {
		cout << array[i] << " ";
	}
	delete[] array;
}


int* generateSecretAiCode(int size) {

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

bool checkSameValue(int value, int arr[], int index) {

	for (int i = 0; i < index; i++) {
		if (value == arr[i]) {
			return true;
		}
	}
	return false;
}