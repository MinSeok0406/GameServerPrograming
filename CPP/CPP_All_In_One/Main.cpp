#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <format>
#include <list>
#include <optional>
#include <map>
#include <set>
#include <iterator>
#include <WinSock2.h>
#include <new>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

class StandardNewAndDelete
{
public:
    // 기본 버전 new
    static void* operator new(size_t size) noexcept
    {
        try
        {
            // 예외 발생 처리

            return ::operator new(size);
        }
        catch (const bad_alloc& e)
        {
        }
    }

    // 기본 버전 delete
    static void operator delete(void* pMemory) noexcept
    {
        try
        {
            // 예외 발생 처리

            ::operator delete(pMemory);
        }
        catch (const exception& e)
        {
        }
    }

    // 위치지정 new
    static void* operator new(size_t size, void* ptr) noexcept
    {
        try
        {
            // 예외 발생 처리

            return ::operator new(size, ptr);
        }
        catch (const exception& e)
        {
        }
    }

    // 위치지정 delete
    static void operator delete(void* pMemory, void* ptr) noexcept
    {
        try
        {
            // 예외 발생 처리

            ::operator delete(pMemory, ptr);
        }
        catch (const exception& e)
        {
        }
    }

    // 위치지정 new
    static void* operator new(size_t size, const nothrow_t& nt) noexcept
    {
        try
        {
            // 예외 발생 처리

            return ::operator new(size, nt);
        }
        catch (const exception& e)
        {
        }
    }

    // 위치지정 delete
    static void operator delete(void* pMemory, const nothrow_t& t) noexcept
    {
        try
        {
            // 예외 발생 처리

            ::operator delete(pMemory);
        }
        catch (const exception& e)
        {
        }
    }
};


class Widget : public StandardNewAndDelete
{
public:
    using StandardNewAndDelete::operator new;
    using StandardNewAndDelete::operator delete;

    static void* operator new(size_t size, ostream& logStream)
    {
        cout << "위치지정 new" << "\n";
        return 0;
    }

    static void operator delete(void* pMemory, size_t size) noexcept
    {
        cout << "기본형 delete" << "\n";
    }

    static void operator delete(void* pMemory, ostream& logStream) noexcept
    {
        cout << "위치지정 delete" << "\n";
    }
};

int main()
{
    

    return 0;
}
