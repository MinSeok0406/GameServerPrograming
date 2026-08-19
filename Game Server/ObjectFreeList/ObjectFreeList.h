#pragma once
#include <vector>

// 가비지 컬렉터 만들기 -> 오랫동안 사용되지 않는 메모리 삭제
// 노드가 메모리 침법을 했을 시 파악할 수 있는 코드 작성

enum class OBJECTSIZE
{
    B8 =        8,
    B16 =       16,
    B32 =       32,
    B64 =       64,
    B128 =      128,
    B256 =      256,
    B512 =      512,
    KB1 =       1024,
    KB2 =       2 * 1024,
    KB4 =       4 * 1024,
    KB8 =       8 * 1024,
    KB16 =      16 * 1024,
    KB32 =      32 * 1024,
    KB64 =      64 * 1024,
    KB128 =     128 * 1024,
    KB256 =     256 * 1024,
    KB512 =     512 * 1024,
    MB1 =       1024 * 1024
};

uintptr_t getObjectSize(unsigned __int64 objectsize);

template<typename T>
class ObjectFreeList
{
public:

    ObjectFreeList() { }
    ObjectFreeList(int objectCount, bool isPlacementNew);
    virtual ~ObjectFreeList();

    T* Alloc();
    void Free(T* object);

    /*void* Alloc(int objectsize);
    void Free(void* object, int objectsize);*/

private:
    ObjectFreeList(const ObjectFreeList& obj) = delete;
    ObjectFreeList& operator=(const ObjectFreeList& rhs) = delete;
    ObjectFreeList(ObjectFreeList&& obj) = delete;
    ObjectFreeList& operator=(ObjectFreeList&& rhs) = delete;

    #pragma pack(1)
    struct Node
    {
        uintptr_t cookie_front;
        T data;
        uintptr_t cookie_end;
        Node* next;
    };

    Node* _head;
    uintptr_t _countPool;
    uintptr_t _countAlloc;
    uintptr_t _countFree;
    uintptr_t _cookieValue;
    int _objectCount;
    bool _isplacementNew;
};

template<typename T>
ObjectFreeList<T>::ObjectFreeList(int objectCount, bool isPlacementNew)
    : _objectCount(objectCount), _isplacementNew(isPlacementNew), _countPool(0),
    _countAlloc(0), _countFree(0)
{
    _head = nullptr;
    _cookieValue = (uintptr_t)this;

    for (auto i = 0; i < objectCount; ++i)
    {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->cookie_front = _cookieValue;
        newNode->cookie_end = _cookieValue;

        if (isPlacementNew == false)
        {
            T* data = new (&(newNode->data)) T;
        }

        newNode->next = _head;
        _head = newNode;
        _countPool++;
    }
}

template<typename T>
ObjectFreeList<T>::~ObjectFreeList()
{
    Node* deleteNode = _head;
    while (deleteNode != nullptr)
    {
        Node* nextNode = deleteNode->next;
        if (_isplacementNew == false)
        {
            deleteNode->data.~T();
        }

        free(deleteNode);
        deleteNode = nextNode;
        _countPool--;
    }
}

template<typename T>
T* ObjectFreeList<T>::Alloc()
{
    Node* node = nullptr;
    _countAlloc++;
    if (_head != nullptr)
    {
        node = _head;
        _head = node->next;
        _countPool--;
        if (_isplacementNew)
        {
            T* data = new (&(node->data)) T;
        }
        
        return &(node->data);
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->cookie_front = _cookieValue;
    newNode->cookie_end = _cookieValue;
    newNode->next = nullptr;
    if (_isplacementNew == false)
    {
        T* data = new (&(newNode->data)) T;
    }

    return &(newNode->data);
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
    Node* node = (Node*)((char*)object - 8);
    if (node->cookie_front != _cookieValue || node->cookie_end != _cookieValue)
    {
        __debugbreak();
        return;
    }

    _countFree++;
    if (_isplacementNew)
    {
        node->data.~T();
    }

    node->next = _head;
    _head = node;
    _countPool++;
}

/*template<typename T>
void* ObjectFreeList<T>::Alloc(int objectsize)
{
    unsigned __int64 size = getObjectSize(objectsize);

    if (_head == nullptr)
    {
        _capacity += size;
        Node* temp = (Node*)malloc(size);
        temp->next = _head;
        _head = temp;

        _chunk.push_back({ temp, size });
    }
    else if (_size >= _capacity - size)
    {
        _capacity += size;
        Node* temp = (Node*)malloc(size);
        temp->next = _head;
        _head = temp;

        _chunk.push_back({ temp, size });
    }

    Node* node = _head;
    _head = node->next;
    _size += size;

    return (char*)node;
}

template<typename T>
void ObjectFreeList<T>::Free(void* object, int objectsize)
{
    if (object == nullptr)
    {
        return;
    }

    char* target = (char*)object;
    bool isbelong = false;

    for (const auto& chunk : _chunk)
    {
        char* begin = (char*)chunk.begin;
        char* end = begin + chunk.size;

        if (target >= begin && target < end && (target - begin) % sizeof(Node) == 0)
        {
            isbelong = true;
            break;
        }
    }

    if (!isbelong)
    {
        __debugbreak();
        return;
    }

    Node* node = (Node*)object;
    node->next = _head;
    _head = node;
}*/