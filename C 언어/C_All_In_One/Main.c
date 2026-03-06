#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(void)
{
    FILE* fp = NULL;
    char* pszData = "01234567890123456789012345678901";

    fopen_s(&fp, "Test.dat", "wb");
    if (fp == NULL)
    {
        return 0;
    }

    fwrite(pszData, 32, 1, fp);

    fseek(fp, 5, SEEK_SET);
    fwrite("Hello", 5, 1, fp);

    fclose(fp);

    return 0;
}