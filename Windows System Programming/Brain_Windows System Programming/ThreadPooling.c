/*#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define WORK_MAX    10000
#define THREAD_MAX  50

typedef void (*WORK) (void);

DWORD AddWorkToPool(WORK work);
WORK GetWorkFromPool(void);
DWORD MakeThreadToPool(DWORD numOfThread);
void WorkerThreadFunction(LPVOID pParam);

typedef struct __WorkerThread
{
    HANDLE hThread;
    DWORD idThread;
} WorkerThread;

struct __ThreadPool
{
    WORK workList[WORK_MAX];

    WorkerThread workerThreadList[THREAD_MAX];
    HANDLE workerEventList[THREAD_MAX];

    DWORD idxOfCurrentWork;
    DWORD idxOfLastAddedWork;

    DWORD threadIdx;
} gThreadPool;


static HANDLE mutex = NULL;
HANDLE mutex2 = NULL;

void InitMutex(void)
{
    mutex = CreateMutex(NULL, FALSE, NULL);
}

void DeInitMutex(void)
{
    BOOL ret = CloseHandle(mutex);
}

void Acquiremutex(void)
{
    DWORD ret = WaitForSingleObject(mutex, INFINITE);
    if (ret == WAIT_FAILED)
    {
        _tprintf(_T("Error Occur! \n"));
    }
}

void Releasemutex(void)
{
    BOOL ret = ReleaseMutex(mutex);
    if (ret == 0)
    {
        _tprintf(_T("Error Occur! \n"));
    }
}



DWORD AddWorkToPool(WORK work)
{
    Acquiremutex();
    if (gThreadPool.idxOfLastAddedWork >= WORK_MAX)
    {
        _tprintf(_T("AddWorkToPool fail! \n"));
        return NULL;
    }

    gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;

    for (DWORD i = 0; i < gThreadPool.threadIdx; i++)
    {
        SetEvent(gThreadPool.workerEventList[i]);
    }

    Releasemutex();
    return 1;
}

WORK GetWorkFromPool(void)
{
    WORK work = NULL;

    Acquiremutex();
    if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork))
    {
        Releasemutex();
        return NULL;
    }

    work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];
    Releasemutex();

    return work;
}

DWORD MakeThreadToPool(DWORD numOfThread)
{
    InitMutex();
    DWORD capacity = WORK_MAX - (gThreadPool.threadIdx);

    if (capacity < numOfThread)
    {
        numOfThread = capacity;
    }

    for (DWORD i = 0; i < numOfThread; i++)
    {
        DWORD idThread;
        HANDLE hThread;

        gThreadPool.workerEventList[gThreadPool.threadIdx] = CreateEvent(NULL, FALSE, FALSE, NULL);

        hThread = (HANDLE)_beginthreadex(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunction,
            (LPVOID)gThreadPool.threadIdx, 0, &idThread);

        gThreadPool.workerThreadList[gThreadPool.threadIdx].hThread = hThread;
        gThreadPool.workerThreadList[gThreadPool.threadIdx].idThread = idThread;

        gThreadPool.threadIdx++;
    }

    return numOfThread;
}

void WorkerThreadFunction(LPVOID pParam)
{
    WORK workFunction;
    HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];

    while (1)
    {
        workFunction = GetWorkFromPool();
        if (workFunction == NULL)
        {
            WaitForSingleObject(event, INFINITE);
            continue;
        }
        workFunction();
    }
}


void TestFunction()
{
    WaitForSingleObject(mutex2, INFINITE);
    static int i = 0;
    i++;

    _tprintf(_T("Good Test --%d : Processing thread: %d--\n\n"), i, GetCurrentThreadId());
    ReleaseMutex(mutex2);
}

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    MakeThreadToPool(3);

    mutex2 = CreateMutex(NULL, FALSE, NULL);

    for (int i = 0; i < 100; i++)
    {
        AddWorkToPool(TestFunction);
    }

    CloseHandle(mutex2);

    Sleep(50000);
    return 0;
}*/