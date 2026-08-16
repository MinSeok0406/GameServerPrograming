#pragma once
#include <stack>

template<typename T>
class ObjectFreeList
{
public:
    ObjectFreeList() { }
    ObjectFreeList(int objectCount, bool isPlacementNew);
    virtual ~ObjectFreeList();

    template<typename... Args>
    T* Alloc(Args&&... args);
    void Free(T* object);

private:
    ObjectFreeList(const ObjectFreeList& obj) = delete;
    ObjectFreeList& operator=(const ObjectFreeList& rhs) = delete;
    ObjectFreeList(ObjectFreeList&& obj) = delete;
    ObjectFreeList& operator=(ObjectFreeList&& rhs) = delete;

    struct Node
    {
        Node* next;
    };

    T* _pool;
    Node* _head;
    int _objectCount;
    bool _isplacementNew;
};

template<typename T>
ObjectFreeList<T>::ObjectFreeList(int objectCount, bool isPlacementNew)
    : _objectCount(objectCount), _isplacementNew(isPlacementNew)
{
    _pool = (T*)malloc(sizeof(T) * objectCount);
    _head = nullptr;

    for (auto i = 0; i < objectCount; ++i)
    {
        T* slot = (_pool + i);

        if (isPlacementNew == false)
        {
            new (slot) T();
        }

        Node* node = (Node*)slot;
        node->next = _head;
        _head = node;
    }
}

template<typename T>
ObjectFreeList<T>::~ObjectFreeList()
{
    if (_isplacementNew == false)
    {
        for (auto i = 0; i < _objectCount; ++i)
        {
            (_pool + i)->~T();
        }
    }

    free(_pool);
}

template<typename T>
template<typename... Args>
T* ObjectFreeList<T>::Alloc(Args&&... args)
{
    if (_head == nullptr)
    {
        return nullptr;
    }

    Node* node = _head;
    _head = node->next;

    if (_isplacementNew)
    {
        return new (node) T(std::forward<Args>(args)...);
    }
    else
    {
        return (T*)node;
    }
}

template<typename T>
void ObjectFreeList<T>::Free(T* object)
{
    if (object == nullptr)
    {
        return;
    }
    // T 타입이 다형성을 가지고 독립적으로 존재하는 두 개의 풀에서
    // 서로 다른 풀에서 Free를 하는 사고를 방지하는 코드가 들어가야함

    if (_isplacementNew)
    {
        object->~T();
    }

    Node* node = (Node*)object;
    node->next = _head;
    _head = node;
}