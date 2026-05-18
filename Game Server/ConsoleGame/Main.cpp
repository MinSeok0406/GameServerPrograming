#include <iostream>
#include <string>
#include <Windows.h>
#include "Console.h"
#include "Buffer.h"
#include "UpdateScene.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")

string str;
SCENE g_scene = SCENE::TITLE;
int g_stage;

char g_stageBuffer[LENGTH];
char g_enemyBuffer[LENGTH];

int main()
{
    timeBeginPeriod(1);
    cs_Initial();
    srand((unsigned int)time(nullptr));

    FILE* fp;
    fopen_s(&fp, "../Stage/Stage.txt", "rb");

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    auto readBytes = fread(g_stageBuffer, sizeof(char), fileSize, fp);
    g_stageBuffer[readBytes] = '\0';

    int cnt = 0;
    while (g_stageBuffer[cnt] != '\r')
    {
        str += g_stageBuffer[cnt];
        cnt++;
    }

    fseek(fp, str.size() + 2, SEEK_SET);
    readBytes = fread(g_stageBuffer, sizeof(char), fileSize, fp);
    g_stageBuffer[readBytes] = '\0';

    fclose(fp);

    //--------------------------------

    FILE* efp;
    fopen_s(&efp, "../Enemy/Enemy.txt", "rb");

    fseek(efp, 0, SEEK_END);
    long efileSize = ftell(efp);
    fseek(efp, 0, SEEK_SET);

    readBytes = fread(g_enemyBuffer, sizeof(char), efileSize, efp);
    g_enemyBuffer[readBytes] = '\0';

    fclose(efp);

    while (1)
    {
        switch (g_scene)
        {
        case SCENE::TITLE:
            UpdateTitle();
            break;
        case SCENE::LOAD:
            UpdateLoad();
            break;
        case SCENE::GAME:
            UpdateGame();
            break;
        case SCENE::CLEAR:
            UpdateClear();
            break;
        case SCENE::OVER:
            UpdateOver();
            break;
        }
    }

    return 0;
}