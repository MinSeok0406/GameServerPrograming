#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <time.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
using namespace std;
using namespace rapidjson;
using ll = long long;

#pragma comment(lib, "Winmm.lib")


int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    StringBuffer stringJson;
    Writer<StringBuffer, UTF16<>> writer(stringJson);
    writer.StartObject();
    writer.String(L"Account");
    writer.StartArray();
    {
        writer.StartObject();
        writer.String(L"AccountNo");
        writer.Uint64(1111);
        writer.String(L"Nickname");
        writer.String(L"TestAccount1");
        writer.EndObject();
    }

    {
        writer.StartObject();
        writer.String(L"AccountNo");
        writer.Uint64(2222);
        writer.String(L"Nickname");
        writer.String(L"TestAccount2");
        writer.EndObject();
    }

    {
        writer.StartObject();
        writer.String(L"AccountNo");
        writer.Uint64(3333);
        writer.String(L"Nickname");
        writer.String(L"TestAccount3");
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();

    const char* pJson = stringJson.GetString();
    FILE* file;
    fopen_s(&file, "JSON.txt", "wb");

    auto size = strlen(pJson);
    fwrite(pJson, size * sizeof(char), 1, file);
    fclose(file);
        
    return 0;
}