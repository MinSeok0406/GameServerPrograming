#include <stdio.h>
#include <tchar.h>
#include <windows.h>

void TYPE(TCHAR* fileName)
{
    TCHAR StringBuff[1024];

    FILE* filePtr = _tfopen(fileName, _T("rt, ccs=UTF-8"));
    while (_fgetts(StringBuff, 1024, filePtr))
    {
        _fputts(StringBuff, stdout);
    }
    printf("\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    if (argc < 2)
    {
        return -1;
    }

    TYPE(argv[1]);

    return 0;
}