#include <iostream>
#include "secret_code.h"

using namespace std;

int main() {

	/*int* aiSecretCode = generateSecretAiCode(4);
	delete aiSecretCode;*/

	int size = 4;
	int* playerSecretCode = getPlayerSecretCode(size);

	for (int i = 0; i < size; i++) {
		cout << playerSecretCode[i] << " ";
	}
	return 0;
}