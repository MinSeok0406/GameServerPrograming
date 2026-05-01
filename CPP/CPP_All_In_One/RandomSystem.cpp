/*#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
using namespace std;

#define ITEMCOUNT 5

static int num = 0;

struct Gachar
{
    TCHAR name[100];
    int rate;
};

Gachar gTable[ITEMCOUNT] = {
    {_T("언커먼 갑옷"), 500},
    {_T("레어 투구"), 100},
    {_T("유니크 신발"), 20},
    {_T("레전더리 장신구"), 5},
    {_T("에픽 칼"), 1}
};

TCHAR itemTable[626][20];

const Gachar* GetRandomItem()
{
    int sum = 0;
    for (auto i = 0; i < sizeof(gTable) / sizeof(Gachar); i++)
    {
        sum += gTable[i].rate;
    }

    auto randomValue = (rand() % sum) + 1;

    int acc = 0;
    for (auto i = 0; i < ITEMCOUNT; i++)
    {
        acc += gTable[i].rate;
        if (randomValue <= acc)
        {
            return &gTable[i];
        }
    }

    return nullptr;
}

void GetRandom()
{
    const Gachar* item = GetRandomItem();
    wmemcpy_s(itemTable[num], sizeof(itemTable[num]), item->name, _tcslen(item->name) + 1);
    ++num;

    return;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    srand((unsigned int)time(nullptr));

    for (auto i = 0; i < 626; i++)
    {
        GetRandom();
    }

    for (auto i = 0; i < 626; i++)
    {
        _tprintf(_T("%d번째 아이템 획득 : %s\n"), i + 1, itemTable[i]);
    }

    return 0;
}*/