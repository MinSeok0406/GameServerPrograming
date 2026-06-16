/*#include <iostream>
#include <tchar.h>
#include <WinSock2.h>
#include <conio.h>
#include "StartPrint.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

#ifdef UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tcerr std::wcerr
#define tclog std::wclog
#else
#define tcout std::cout
#define tcin std::cin
#define tcerr std::cerr
#define tclog std::clog
#endif

#ifdef _UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tcerr std::wcerr
#define tclog std::wclog
#else
#define tcout std::cout
#define tcin std::cin
#define tcerr std::cerr
#define tclog std::clog
#endif

IBaseObject* g_ibo[20];

void KeyProcess()
{
    if (_kbhit())
    {
        char ckey = _getch();
        int key = ckey - '0';
        for (auto i = 0; i < 20; ++i)
        {
            if (g_ibo[i] == nullptr)
            {
                switch (key)
                {
                case 1:
                    g_ibo[i] = new OneStar(i);
                    break;
                case 2:
                    g_ibo[i] = new TwoStar(i);
                    break;
                case 3:
                    g_ibo[i] = new ThreeStar(i);
                    break;
                }

                break;
            }
            else if (g_ibo[i]->GetRelease() == true)
            {
                switch (key)
                {
                case 1:
                    delete g_ibo[i];
                    g_ibo[i] = new OneStar(i);
                    break;
                case 2:
                    delete g_ibo[i];
                    g_ibo[i] = new TwoStar(i);
                    break;
                case 3:
                    delete g_ibo[i];
                    g_ibo[i] = new ThreeStar(i);
                    break;
                }

                break;
            }
        }
    }
}

void Update()
{
    for (auto i = 0; i < 20; ++i)
    {
        if (g_ibo[i] == nullptr)
        {
            continue;
        }

        g_ibo[i]->Update();
    }
}

void Render()
{
    for (auto i = 0; i < 20; ++i)
    {
        if (g_ibo[i] == nullptr)
        {
            continue;
        }

        g_ibo[i]->Render();

        cout << "\n";
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);

    while (1)
    {
        KeyProcess();

        Update();

        system("cls");
        Render();

        Sleep(50);
    }

    return 0;
}*/