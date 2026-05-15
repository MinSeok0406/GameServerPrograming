#include <iostream>
#include "Console.h"
#include "Buffer.h"
#include "UpdateScene.h"
using namespace std;

#define LENGTH 1024

SCENE g_scene = SCENE::TITLE;
int g_state;

char g_stageBuffer[LENGTH];

int main()
{
    cs_Initial();

    FILE* fp;
    fopen_s(&fp, "../Stage/Stage.txt", "rb");
    fread(g_stageBuffer, sizeof(char), LENGTH, fp);

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

    fclose(fp);

    return 0;
}