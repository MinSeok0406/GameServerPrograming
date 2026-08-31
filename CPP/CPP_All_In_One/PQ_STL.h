#pragma once
#include <vector>
// 힙 트리
// 1. 부모 노드가 자식 노드보다 값이 커야함 (최대 힙 정렬)
// 2. 완전 이진 트리

// i 번 노드의 왼쪽 자식은 [(2 * i) + 1] 번
// i 번 노드의 오른쪽 자식은 [(2 * i) + 2] 번
// i 번 노드의 부모는 [(i - 1) / 2] 번

// 힙 트리 Push
// 노드 삽입 -> 힙 트리 규칙

// 힙 트리 Pop
// 루트 노드 추출 -> 마지막 노드를 루트 노드로 옮기고 힙 트리 규칙

template<typename T, bool isArray = true>
class PriorityQueue
{
public:
    bool Push(T data);
    bool Pop();
    bool Empty();
    T Top();
    int Size();

    void Swap(T* a, T* b);

private:
    std::vector<T> tree;
};

template<typename T, bool isArray>
bool PriorityQueue<T, isArray>::Push(T data)
{
    tree.push_back(data);

    // 힙 트리 규칙
    if (isArray == true)
    {
        int index = (int)(tree.size() - 1);
        while (index > 0)
        {
            int parentIndex = (index - 1) / 2;
            if (tree[index] > tree[parentIndex])
            {
                Swap(&tree[index], &tree[parentIndex]);
                index = parentIndex;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        int index = (int)(tree.size() - 1);
        while (index > 0)
        {
            int parentIndex = (index - 1) / 2;
            if (tree[index] < tree[parentIndex])
            {
                Swap(&tree[index], &tree[parentIndex]);
                index = parentIndex;
            }
            else
            {
                break;
            }
        }
    }

    return true;
}

template<typename T, bool isArray>
bool PriorityQueue<T, isArray>::Pop()
{
    if (tree.empty())
    {
        return false;
    }
    tree[0] = tree[tree.size() - 1];
    tree.pop_back();

    // 힙 트리 규칙
    if (isArray == true)
    {
        int index = 0;
        while (true)
        {
            int leftIndex = (2 * index) + 1;
            int rightIndex = (2 * index) + 2;

            if (leftIndex < tree.size() && rightIndex < tree.size())
            {
                if (tree[index] >= tree[leftIndex] && tree[index] >= tree[rightIndex])
                {
                    break;
                }

                if (tree[leftIndex] >= tree[rightIndex])
                {
                    Swap(&tree[index], &tree[leftIndex]);
                    index = leftIndex;
                }
                else
                {
                    Swap(&tree[index], &tree[rightIndex]);
                    index = rightIndex;
                }
            }
            else if (leftIndex < tree.size())
            {
                if (tree[leftIndex] >= tree[index])
                {
                    Swap(&tree[index], &tree[leftIndex]);
                    index = leftIndex;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        int index = 0;
        while (true)
        {
            int leftIndex = (2 * index) + 1;
            int rightIndex = (2 * index) + 2;

            if (leftIndex < tree.size() && rightIndex < tree.size())
            {
                if (tree[index] <= tree[leftIndex] && tree[index] <= tree[rightIndex])
                {
                    break;
                }

                if (tree[leftIndex] >= tree[rightIndex])
                {
                    Swap(&tree[index], &tree[rightIndex]);
                    index = rightIndex;
                }
                else
                {
                    Swap(&tree[index], &tree[leftIndex]);
                    index = leftIndex;
                }
            }
            else if (leftIndex < tree.size())
            {
                if (tree[leftIndex] <= tree[index])
                {
                    Swap(&tree[index], &tree[leftIndex]);
                    index = leftIndex;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    return true;
}

template<typename T, bool isArray>
T PriorityQueue<T, isArray>::Top()
{
    if (tree.empty())
    {
        return -1;
    }

    return tree[0];
}

template<typename T, bool isArray>
inline bool PriorityQueue<T, isArray>::Empty()
{
    return (Size() == 0);
}

template<typename T, bool isArray>
inline int PriorityQueue<T, isArray>::Size()
{
    return (int)tree.size();
}

template<typename T, bool isArray>
inline void PriorityQueue<T, isArray>::Swap(T* a, T* b)
{
    T temp = *a;
    *a = *b;
    *b = temp;
}