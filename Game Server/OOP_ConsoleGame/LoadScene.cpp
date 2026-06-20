#include "LoadScene.h"
#include "ManagerObject.h"
#include "SceneManager.h"
#include <string>
#include <vector>
using namespace std;

struct Enemy
{
    bool live;
    bool isMove;
    char sprite;
    int x;
    int y;
    int moveX;
    int moveY;
};

vector<Enemy> enemies;

extern SceneManager* g_sceneManager;
extern ManagerObject* g_managerObject;
extern int g_stage;
extern char buffer[256];
extern char enemyInfo[256][6];
extern int enemyTypeCnt;
extern char g_stageBuffer[LENGTH];
extern string str;

char buffer[256];
int g_stageBufferOffset = 0;

LoadScene::LoadScene()
{}

LoadScene::~LoadScene()
{}

bool LoadScene::Update()
{
    int maxStage = stoi(str);

    // 개발자가 만든 스테이지를 초과한다면...
    if (g_stage > maxStage)
    {
        g_sceneManager->loadScene(SCENE::CLEAR);
        return true;
    }

    g_managerObject->_objectList.clear();

    // 스테이지에 맞는 정보 읽기
    string arr;
    while (g_stageBuffer[g_stageBufferOffset] != '\0')
    {
        if (g_stageBuffer[g_stageBufferOffset] == '\0')
        {
            break;
        }

        char c = g_stageBuffer[g_stageBufferOffset];
        g_stageBufferOffset++;

        if (c == '\r')
        {
            if (g_stageBuffer[g_stageBufferOffset] == '\n')
            {
                g_stageBufferOffset++;
            }
            break;
        }
        arr += c;
    }

    FILE* fp;
    fopen_s(&fp, arr.c_str(), "rb");

    // 작성된 스테이지 텍스트 파일이 없다면...
    if (fp == nullptr)
    {
        g_sceneManager->loadScene(SCENE::CLEAR);
        return true;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    auto readBytes = fread(buffer, sizeof(char), fileSize, fp);
    buffer[readBytes] = '\0';

    fclose(fp);

    g_managerObject->CreateObject(OBJECT_TYPE::PLAYER);
    EnemyInit();

    g_sceneManager->loadScene(SCENE::GAME);

    return true;
}

void LoadScene::EnemyInit()
{
    // Stage에 나오는 스프라이트 좌표 수집
    int x = 0;
    int y = 0;
    int cnt = 0;
    while (buffer[cnt] != '\0')
    {
        char c = buffer[cnt];

        if (c == '\r' || c == '\n')
        {
            if (c == '\r')
            {
                cnt += 2;
            }
            x = 0;
            y++;
            continue;
        }

        if (c == '@' || c == 'a' || c == 'Q' || c == 'W' || c == 'V')
        {
            enemies.push_back({ true, true, c, x, y, 0, 0 });
        }

        x++;
        cnt++;
    }

    // Enemy 이동 패턴 값 설정
    char sprite;
    int moveX;
    int moveY;

    for (auto i = 0; i < enemyTypeCnt; ++i)
    {
        sscanf_s(enemyInfo[i], " %c %d %d", &sprite, 1, &moveX, &moveY);

        for (auto& enemy : enemies)
        {
            if (enemy.sprite == sprite)
            {
                enemy.moveX = moveX;
                enemy.moveY = moveY;
            }
        }
    }

    for (auto& enemy : enemies)
    {
        g_managerObject->CreateObject(OBJECT_TYPE::ENEMY,
            enemy.live, enemy.isMove, enemy.sprite, enemy.x, enemy.y,
            enemy.moveX, enemy.moveY);
    }
}