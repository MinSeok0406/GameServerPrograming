#include <iostream>
#include "Console.h"
#include "Buffer.h"
#include "UpdateScene.h"
using namespace std;

SCENE g_scene = SCENE::TITLE;
int g_state;

int main()
{
    cs_Initial();

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