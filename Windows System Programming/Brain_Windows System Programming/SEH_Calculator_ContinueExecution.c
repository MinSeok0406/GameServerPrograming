/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

enum
{
    DIV = 1,
    MUL,
    ADD,
    MIN,
    EXIT
};

DWORD ShowMenu();
BOOL Calculator(void);
DWORD FilterFunction(DWORD exptType);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    BOOL state;

    do
    {
        state = Calculator();
    } while (state == TRUE);


    return 0;
}

DWORD ShowMenu()
{
    DWORD sel;

    _fputts(_T("-----Menu----- \n"), stdout);
    _fputts(_T("num 1: Divide \n"), stdout);
    _fputts(_T("num 2: Multiple \n"), stdout);
    _fputts(_T("num 3: Add \n"), stdout);
    _fputts(_T("num 4: Minus \n"), stdout);
    _fputts(_T("num 5: Exit \n"), stdout);
    _fputts(_T("SELECTION >>"), stdout);
    _tscanf_s(_T("%d"), &sel);

    return sel;
}

int num1, num2, result;

BOOL Calculator(void)
{
    DWORD sel;
    sel = ShowMenu();
    if (sel == EXIT)
    {
        return FALSE;
    }

    _fputts(_T("Input Num1 Num2: "), stdout);
    _tscanf_s(_T("%d %d"), &num1, &num2);

    __try
    {
        switch (sel)
        {
        case DIV:
            result = num1 / num2;
            _tprintf(_T("%d / %d = %d \n\n"), num1, num2, result);
            break;
        case MUL:
            result = num1 * num2;
            _tprintf(_T("%d * %d = %d \n\n"), num1, num2, result);
            break;
        case ADD:
            result = num1 + num2;
            _tprintf(_T("%d + %d = %d \n\n"), num1, num2, result);
            break;
        case MIN:
            result = num1 - num2;
            _tprintf(_T("%d - %d = %d \n\n"), num1, num2, result);
            break;
        }
    }
    __except (FilterFunction(GetExceptionCode()))
    {
        _tprintf(_T("__except block... \n\n"));
    }

    return TRUE;
}

DWORD FilterFunction(DWORD exptType)
{
    switch (exptType)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        _tprintf(_T("Access violation \n\n"));
        return EXCEPTION_EXECUTE_HANDLER;

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        _tprintf(_T("Wrong number inserted. \n"));
        _tprintf(_T("Input second number again: "));
        _tscanf(_T("%d"), &num2);
        return EXCEPTION_CONTINUE_EXECUTION;

    default:
        return EXCEPTION_EXECUTE_HANDLER;
    }
}*/