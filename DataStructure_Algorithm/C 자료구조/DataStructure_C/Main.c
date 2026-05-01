#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct USERDATA
{
    int age;
    char name[32];
    char phone[32];
    struct USERDATA* pNext;
} USERDATA;

USERDATA* g_pHeadNode = NULL;

void AddNewNode(int age, const char* pszName, const char* pszPhone)
{
    USERDATA* pNewNode = (USERDATA*)malloc(sizeof(USERDATA));
    pNewNode->age = age;
    strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
    strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);
    pNewNode->pNext = NULL;

    if (g_pHeadNode == NULL)
    {
        g_pHeadNode = pNewNode;
    }
    else
    {
        USERDATA* pTail = g_pHeadNode;
        while (pTail->pNext != NULL)
        {
            pTail = pTail->pNext;
        }

        pTail->pNext = pNewNode;
    }
}

void ReleaseList(void)
{
    USERDATA* pTmp = g_pHeadNode;
    USERDATA* pDelete;
    while (pTmp != NULL)
    {
        pDelete = pTmp;
        pTmp = pTmp->pNext;
        free(pDelete);
    }

    g_pHeadNode = NULL;
}

USERDATA* SearchByName(const char* pszName)
{
    USERDATA* pTmp = g_pHeadNode;
    while (pTmp != NULL)
    {
        if (strcmp(pTmp->name, pszName) == 0)
        {
            printf("\"%s\": Found \n\n", pszName);
            return pTmp;
        }

        pTmp = pTmp->pNext;
    }

    printf("\"%s\": Not Found \n\n", pszName);
    return NULL;
}

USERDATA* SearchToRemove(USERDATA** ppPrev, const char* pszName)
{
    USERDATA* pCur = g_pHeadNode;
    USERDATA* pPrev = NULL;
    while (pCur != NULL)
    {
        if (strcmp(pCur->name, pszName) == 0)
        {
            *ppPrev = pPrev;
            return pCur;
        }

        pPrev = pCur;
        pCur = pCur->pNext;
    }

    return NULL;
}

void RemoveNode(USERDATA* pPrev)
{
    USERDATA* pRemove = NULL;
    if (pPrev == NULL)
    {
        if (g_pHeadNode == NULL)
        {
            return;
        }
        
        pRemove = g_pHeadNode;
        g_pHeadNode = pRemove->pNext;
        free(pRemove);
    }

    pRemove = pPrev->pNext;
    pPrev->pNext = pRemove->pNext;
    free(pRemove);
}

int main(void)
{
    AddNewNode(10, "Minseok1", "010-1111-1111");
    AddNewNode(20, "Minseok2", "010-2222-2222");
    AddNewNode(30, "Minseok3", "010-3333-3333");
    AddNewNode(40, "Minseok4", "010-4444-4444");

    USERDATA* pPrev = NULL;
    if (SearchToRemove(&pPrev, "Minseok1") != NULL)
    {
        RemoveNode(pPrev);
    }

    SearchByName("Minseok1");
    SearchByName("Minseok2");
    SearchByName("Minseok3");
    SearchByName("Minseok4");

    USERDATA* pTmp = g_pHeadNode;
    while (pTmp != NULL)
    {
        printf("[%p] %d, %s, %s [%p]\n", pTmp, pTmp->age, pTmp->name, pTmp->phone, pTmp->pNext);
        pTmp = pTmp->pNext;
    }

    ReleaseList();

    return 0;
}