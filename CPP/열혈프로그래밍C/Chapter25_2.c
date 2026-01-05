#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 문제 25-2-1
//char* arr;
//char temp[100][100];
//
//void CreateArray(char** arr, int len)
//{
//    *arr = (char*)malloc(sizeof(char) * len);
//    if (*arr == NULL)
//    {
//        return;
//    }
//    
//    fputs("입력해주세요: ", stdout);
//    fgets(*arr, len, stdin);
//}

// 문제 25-2-2
int* arr;

int main(void)
{
    // 문제 25-2-1
    /*int n;
    scanf_s("%d", &n);
    getchar();

    CreateArray(&arr, n);

    int cnt = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    while (arr[cnt] != '\n')
    {
        if (arr[cnt] == ' ')
        {
            cnt3 = 0;
            cnt2++;
            cnt++;
            continue;
        }

        temp[cnt2][cnt3++] = arr[cnt++];
    }

    for (int i = cnt2; i >= 0; i--)
    {
        cnt3 = 0;
        while (temp[i][cnt3] != '\0')
        {
            printf("%c", temp[i][cnt3++]);
        }
        printf(" ");
    }
    
    free(arr);
    */

    // 문제 25-2-2
    arr = (int*)malloc(sizeof(int) * 5);
    if (arr == NULL)
    {
        // TODO
    }

    int n;
    int cnt = 0;
    int cnt2 = 5;
    while (1)
    {
        if (arr[cnt] == '\0')
        {
            cnt2 += 3;
            arr = (int*)realloc(arr, sizeof(int) * cnt2);
        }
        scanf_s("%d", &n);
        if (n == -1)
        {
            break;
        }

        arr[cnt++] = n;
    }


    return 0;
}