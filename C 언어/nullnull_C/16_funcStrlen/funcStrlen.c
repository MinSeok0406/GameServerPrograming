#include <stdio.h>

int GetLength(const char* pszParam) {

	int nLength = 0;
	while (pszParam[nLength] != '\0')
		nLength++;

	return nLength;
}

int main(int argc, char* argv[]) {

	char* pszData = "Hello";

	printf("%d\n", GetLength("Hi"));
	printf("%d\n", GetLength(pszData));
	return 0;
}