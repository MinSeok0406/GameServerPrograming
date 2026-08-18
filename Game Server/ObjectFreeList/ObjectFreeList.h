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

unsigned __int64 getObjectSize(unsigned __int64 objectsize);

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
    void* Alloc(int objectsize);
    void Free(void* object, int objectsize);

private:
    ObjectFreeList(const ObjectFreeList& obj) = delete;
    ObjectFreeList& operator=(const ObjectFreeList& rhs) = delete;
    ObjectFreeList(ObjectFreeList&& obj) = delete;
    ObjectFreeList& operator=(ObjectFreeList&& rhs) = delete;

    struct Chunk
    {
        void* begin;
        unsigned __int64 size;
    };

    struct HEADER
    {
        void* securityCookie;
    };

    struct Node
    {
        HEADER header;
        Node* next;
    };

    T* _pool;
    Node* _head;
    std::vector<Chunk> _chunk;
    unsigned __int64 _size;
    unsigned __int64 _capacity;
    int _objectCount;
    bool _isplacementNew;
};

template<typename T>
ObjectFreeList<T>::ObjectFreeList(int objectCount, bool isPlacementNew)
    : _objectCount(objectCount), _isplacementNew(isPlacementNew), _size(0)
{
    _capacity = sizeof(T) * objectCount;
    _pool = (T*)malloc(_capacity);
    _head = nullptr;

    _chunk.push_back({ _pool, _capacity });

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
        _capacity += sizeof(T);
        Node* temp = (Node*)malloc(sizeof(T));
        temp->next = _head;
        _head = temp;

        _chunk.push_back({ temp, sizeof(T) });
    }
    else if (_size >= _capacity - sizeof(T))
    {
        _capacity += sizeof(T);
        Node* temp = (Node*)malloc(sizeof(T));
        temp->next = _head;
        _head = temp;

        _chunk.push_back({ temp, sizeof(T) });
    }

    Node* node = _head;
    _head = node->next;
    _size += sizeof(T);

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

        if (target >= begin && target < end && (target - begin) % sizeof(T) == 0)
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
    char* target = (char*)object;
    bool isbelong = false;

    for (const auto& chunk : _chunk)
    {
        char* begin = (char*)chunk.begin;
        char* end = begin + chunk.size;

        if (target >= begin && target < end && (target - begin) % sizeof(T) == 0)
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

    if (_isplacementNew)
    {
        object->~T();
    }

    Node* node = (Node*)object;
    node->next = _head;
    _head = node;
}