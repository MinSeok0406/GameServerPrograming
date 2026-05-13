/*#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <strsafe.h>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")

extern "C" const IMAGE_DOS_HEADER __ImageBase;


int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);

    FILE* file1;
    FILE* file2;
    FILE* file3;

    _tfopen_s(&file1, _T("Test.bmp"), _T("wb"));
    _tfopen_s(&file2, _T("sample.bmp"), _T("rb"));
    _tfopen_s(&file3, _T("sample2.bmp"), _T("rb"));

    BITMAPFILEHEADER BFH1;
    BITMAPFILEHEADER BFH2;

    BITMAPINFOHEADER BIH1;
    BITMAPINFOHEADER BIH2;

    fread(&BFH1, sizeof(BITMAPFILEHEADER), 1, file2);
    fread(&BIH1, sizeof(BITMAPINFOHEADER), 1, file2);

    fread(&BFH2, sizeof(BITMAPFILEHEADER), 1, file3);
    fread(&BIH2, sizeof(BITMAPINFOHEADER), 1, file3);

    fwrite(&BFH1, sizeof(BITMAPFILEHEADER), 1, file1);
    fwrite(&BIH1, sizeof(BITMAPINFOHEADER), 1, file1);

    unsigned char pixel1[4];
    unsigned char pixel2[4];
    unsigned char result[4];

    for (auto i = 0; i < BIH1.biHeight; i++)
    {
        for (auto j = 0; j < BIH1.biWidth; j++)
        {
            fread(pixel1, sizeof(unsigned char), 4, file2);
            fread(pixel2, sizeof(unsigned char), 4, file3);

            for (auto k = 0; k < 4; k++)
            {
                result[k] = (pixel1[k] + pixel2[k]) / 2;
            }

            fwrite(result, sizeof(unsigned char), 4, file1);
        }
    }


    fclose(file1);
    fclose(file2);
    fclose(file3);

    return 0;
}*/