#include "Parser.h"

BOOL Parser::LoadFile(const char* str)
{


    return 0;
}

BOOL Parser::GetValue(const char* str, int* value)
{
    char* chpBuff;
    char chWord[256];
    int iLength;

    while (GetNextWord(&chpBuff, &iLength))
    {
        memset(chWord, 0, 256);
        memcpy(chWord, chpBuff, iLength);

        if (0 == strcmp(str, chWord))
        {
            if (GetNextWord(&chpBuff, &iLength))
            {
                memset(chWord, 0, 256);
                memcpy(chWord, chpBuff, iLength);
                if (0 == strcmp(chWord, "="))
                {
                    if (GetNextWord(&chpBuff, &iLength))
                    {
                        memset(chWord, 0, 256);
                        memcpy(chWord, chpBuff, iLength);
                        *value = atoi(chWord);
                        return TRUE;
                    }
                    return FALSE;
                }
            }
            return FALSE;
        }
    }

    return FALSE;
}

BOOL SkipNoneCommand(void)
{
    return 0;
}

BOOL GetNextWord(char** chppBuffer, int* ipLength)
{
    return 0;
}

BOOL GetStringWord(char** chppBuffer, int* ipLength)
{
    return 0;
}
