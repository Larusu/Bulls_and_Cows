#include <iostream>
#include "ai_code.h"

using namespace std;


int main() {

	int* array = generateSecretAiCode(4);

	for (int i = 0; i < 4; i++) {
		cout << array[i] << " ";
	}
	delete array;
}