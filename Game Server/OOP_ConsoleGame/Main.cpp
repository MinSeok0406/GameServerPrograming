#include <iostream>
#include <string>
#include <Windows.h>
#include "Console.h"
#include "Buffer.h"
#include "ManagerObject.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Profile.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")

#define LENGTH 1024

ScreenBuffer* g_screenBuffer = nullptr;
Console* g_console = nullptr;
ManagerObject* g_managerObject = nullptr;
SceneManager* g_sceneManager = nullptr;

string str;
int g_stage;

char g_stageBuffer[LENGTH];
char g_enemyBuffer[LENGTH];
char enemyInfo[256][6];
int g_enemyBufferOffset = 0;
int enemyTypeCnt = 0;

int wmain()
{
    timeBeginPeriod(1);
    g_screenBuffer = ScreenBuffer::getInstance();
    g_console = Console::getInstance();
    g_managerObject = ManagerObject::getInstance();
    g_sceneManager = SceneManager::getInstance();
    srand((unsigned int)time(nullptr));

    g_console->cs_Initial();

    FILE* fp;
    fopen_s(&fp, "Stage/Stage.txt", "rb");

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

    fseek(fp, (long)(str.size() + 2), SEEK_SET);
    readBytes = fread(g_stageBuffer, sizeof(char), fileSize, fp);
    g_stageBuffer[readBytes] = '\0';

    fclose(fp);

    //--------------------------------

    FILE* efp;
    fopen_s(&efp, "Enemy/Enemy.txt", "rb");

    fseek(efp, 0, SEEK_END);
    long efileSize = ftell(efp);
    fseek(efp, 0, SEEK_SET);

    readBytes = fread(g_enemyBuffer, sizeof(char), efileSize, efp);
    g_enemyBuffer[readBytes] = '\0';

    fclose(efp);

    //--------------------------------

    cnt = 0;
    while (g_enemyBuffer[g_enemyBufferOffset] != '\0')
    {
        string arr;
        while (true)
        {
            if (g_enemyBuffer[g_enemyBufferOffset] == '\0')
            {
                break;
            }

            char c = g_enemyBuffer[g_enemyBufferOffset];
            g_enemyBufferOffset++;

            if (c == '\r')
            {
                if (g_enemyBuffer[g_enemyBufferOffset] == '\n')
                {
                    g_enemyBufferOffset++;
                    break;
                }
            }
            arr += c;
        }

        FILE* fp;
        fopen_s(&fp, arr.c_str(), "rb");

        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        auto readBytes = fread(enemyInfo[cnt], sizeof(char), fileSize, fp);
        enemyInfo[cnt][readBytes] = '\0';

        fclose(fp);

        cnt++;
        enemyTypeCnt++;
    }

    g_sceneManager->_pScene = new TitleScene;
    while (1)
    {
        g_sceneManager->run();
    }

    g_screenBuffer->destoryInstance();
    g_console->destoryInstance();
    g_managerObject->destoryInstance();
    g_sceneManager->destoryInstance();

    ProfileDataOutText(L"profile.txt");

    return 0;
}