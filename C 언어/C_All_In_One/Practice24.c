/*#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

void MyStrCpy(char* strDst, int size, char* strSrc)
{
    int cnt = 0;
    while (cnt < size)
    {
        strDst[cnt] = strSrc[cnt];
        cnt++;
    }
}

int main(void)
{
    char szBufferSrc[12] = { "TestString" };
    char szBufferDst[12] = { 0 };

    MyStrCpy(szBufferDst, sizeof(szBufferDst), szBufferSrc);
    puts(szBufferDst);

    return 0;
}*/