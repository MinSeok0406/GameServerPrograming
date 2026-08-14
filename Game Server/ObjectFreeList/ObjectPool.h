#pragma once
#include <numeric>
#include <memory>
#include <vector>

template<typename T, typename Allocator = std::allocator<T>>
class ObjectPool
{
public:
    ObjectPool() = default;
    explicit ObjectPool(const Allocator& allocator);
    virtual ~ObjectPool();

    ObjectPool(ObjectPool&& src) noexcept = default;
    ObjectPool& operator=(ObjectPool&& rhs) noexcept = default;

    ObjectPool(const ObjectPool& src) = delete;
    ObjectPool& operator=(const ObjectPool& rhs) = delete;

    template<typename... Args>
    std::shared_ptr<T> acquireObject(Args... args);

private:
    std::vector<T*> _pool;
    std::vector<T*> _freeObjects;
    static const size_t s_initialChunkSize = 5;
    size_t _newChunkSize = s_initialChunkSize;
    void addChunk();
    Allocator _allocator;
};

template<typename T, typename Allocator>
ObjectPool<T, Allocator>::ObjectPool(const Allocator& allocator)
    : _allocator(allocator)
{}

template<typename T, typename Allocator>
ObjectPool<T, Allocator>::~ObjectPool()
{
    // 메모리 반납을 못한 객체가 있을 시 처리해주는 코드 작성
    // ....
    

    size_t chunkSize = s_initialChunkSize;
    for (auto* chunk : _pool)
    {
        _allocator.deallocate(chunk, chunkSize);
        chunkSize *= 2;
    }
    _pool.clear();
}

template<typename T, typename Allocator>
void ObjectPool<T, Allocator>::addChunk()
{
    auto* firstNewObject = _allocator.allocate(_newChunkSize);
    _pool.push_back(firstNewObject);

    auto oldFreeObjectSize = _freeObjects.size();
    _freeObjects.resize(oldFreeObjectSize + _newChunkSize);
    std::iota(std::begin(_freeObjects) + oldFreeObjectSize, std::end(_freeObjects), firstNewObject);

    _newChunkSize *= 2;
}

template<typename T, typename Allocator>
template<typename ...Args>
std::shared_ptr<T> ObjectPool<T, Allocator>::acquireObject(Args ...args)
{
    if (_freeObjects.empty())
    {
        addChunk();
    }

    T* object = _freeObjects.back();
    new(object) T { std::forward<Args>(args)... };
    _freeObjects.pop_back();

    return std::shared_ptr<T> {object, [this](T* object) 
        {
        std::destroy_at(object);
        _freeObjects.push_back(object);
        }
    };
}
