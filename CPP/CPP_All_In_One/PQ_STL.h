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

template<typename T, typename Container = std::vector<T>, typename Pre = std::less<typename Container::value_type>>
class PriorityQueue
{
public:
    bool Push(const T& data);
    bool Pop();
    bool Empty();
    const T& Top();
    int Size();

    void Swap(T* a, T* b);

private:
    Container tree {};
    Pre comp {};
};

// 클로드 최적화 코드
template<typename T, typename Container, typename Pre>
bool PriorityQueue<T, Container, Pre>::Push(const T& data)
{
    tree.push_back(T {});
    int index = (int)(tree.size() - 1);
    int parentIndex = (index - 1) / 2;

    while (index > 0 && comp(tree[parentIndex], data))
    {
        tree[index] = std::move(tree[parentIndex]); // swap 대신 이동 1번
        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
    tree[index] = data;
    return true;
}

// 클로드 최적화 코드
template<typename T, typename Container, typename Pre>
bool PriorityQueue<T, Container, Pre>::Pop()
{
    if (tree.empty()) return false;

    T last = std::move(tree.back());
    tree.pop_back();
    if (tree.empty()) return true;

    int size = (int)tree.size();
    int index = 0;

    // 1단계: 큰 자식을 계속 끌어올리며 리프까지 (레벨당 비교 1번)
    while (true)
    {
        int leftIndex = 2 * index + 1;
        int rightIndex = 2 * index + 2;
        if (leftIndex >= size) break;

        int biggerIndex = leftIndex;
        if (rightIndex < size && comp(tree[leftIndex], tree[rightIndex]))
            biggerIndex = rightIndex;

        tree[index] = std::move(tree[biggerIndex]);
        index = biggerIndex;
    }

    // 2단계: 마지막 원소를 제자리로 끌어올림
    int parentIndex = (index - 1) / 2;
    while (index > 0 && comp(tree[parentIndex], last))
    {
        tree[index] = std::move(tree[parentIndex]);
        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
    tree[index] = std::move(last);
    return true;
}

/*template<typename T, typename Container, typename Pre>
bool PriorityQueue<T, Container, Pre>::Push(const T& data)
{
    tree.push_back(data);

    // 힙 트리 규칙
    int index = (int)(tree.size() - 1);
    while (index > 0)
    {
        int parentIndex = (index - 1) / 2;
        if (comp(tree[index], tree[parentIndex]))
        {
            break;
        }

        Swap(&tree[index], &tree[parentIndex]);
        index = parentIndex;
    }

    return true;
}

template<typename T, typename Container, typename Pre>
bool PriorityQueue<T, Container, Pre>::Pop()
{
    if (tree.empty())
    {
        return false;
    }
    tree[0] = tree[tree.size() - 1];
    tree.pop_back();

    // 힙 트리 규칙
    int index = 0;
    while (true)
    {
        int leftIndex = (2 * index) + 1;
        int rightIndex = (2 * index) + 2;

        if (leftIndex < tree.size() && rightIndex < tree.size())
        {
            if (comp(tree[leftIndex], tree[index]) && comp(tree[rightIndex], tree[index]))
            {
                break;
            }

            if (comp(tree[leftIndex], tree[rightIndex]))
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
            if (comp(tree[index], tree[leftIndex]))
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

    return true;
}*/

template<typename T, typename Container, typename Pre>
const T& PriorityQueue<T, Container, Pre>::Top()
{
    if (tree.empty())
    {
        __debugbreak();
    }

    return tree[0];
}

template<typename T, typename Container, typename Pre>
inline bool PriorityQueue<T, Container, Pre>::Empty()
{
    return (Size() == 0);
}

template<typename T, typename Container, typename Pre>
inline int PriorityQueue<T, Container, Pre>::Size()
{
    return (int)tree.size();
}

template<typename T, typename Container, typename Pre>
inline void PriorityQueue<T, Container, Pre>::Swap(T* a, T* b)
{
    T temp = *a;
    *a = *b;
    *b = temp;
}