#pragma once

#define NOMINMAX
#include <Windows.h>

#include "ObjectFreeList.h"

// [방어 코드] ObjectFreeList.h는 #pragma pack(1)만 걸고 pop을 하지 않으므로,
// 그 뒤에 선언되는 모든 구조체(바로 아래의 TestObject 포함)가 1바이트로
// 강제 패킹된 채로 컴파일됩니다. 이걸 그대로 두면 TestObject 자체가
// 오염되어 "정상적인 T라면 원래 몇 바이트 정렬이 필요했는지"를 알 수 없게
// 되므로, 여기서 정렬을 원래 기본값으로 되돌립니다.
// (원본 코드에서는 #pragma pack(push, 1) / #pragma pack(pop) 쌍으로
//  Node 선언만 감쌌어야 이 문제가 생기지 않습니다.)
#pragma pack()

#include <iostream>
#include <ostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <atomic>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdint>

// =====================================================================
// 계측용 테스트 오브젝트
//  - 생성자에서 LIVE_MAGIC을 심고, 소멸자에서 DEAD_MAGIC으로 덮어씀
//  - Stamp()/IsCanaryIntact()로 "생성자가 실제로 돌았는지", "메모리가
//    다른 곳에서 덮어써지지 않았는지"를 매 연산마다 검증할 수 있음
// =====================================================================
struct TestObject
{
    static constexpr uint32_t LIVE_MAGIC = 0xC0FFEE11u;
    static constexpr uint32_t DEAD_MAGIC = 0xDEADBEEFu;

    static std::atomic<int64_t> s_ctorCount;
    static std::atomic<int64_t> s_dtorCount;

    uint32_t magic;
    uint64_t id;
    uint64_t payload[6];

    TestObject()
        : magic(LIVE_MAGIC), id(0)
    {
        for (auto& p : payload) p = 0xAAAAAAAAAAAAAAAAull;
        s_ctorCount.fetch_add(1, std::memory_order_relaxed);
    }

    ~TestObject()
    {
        // 이미 죽었다는 표식을 남겨서, 이후 use-after-free를 더 잘 드러나게 함
        magic = DEAD_MAGIC;
        s_dtorCount.fetch_add(1, std::memory_order_relaxed);
    }

    void Stamp(uint64_t newId)
    {
        id = newId;
        for (auto& p : payload) p = newId;
    }

    // magic이 LIVE_MAGIC인지, id/payload가 우리가 마지막으로 찍은 값과
    // 일치하는지를 확인. 생성자 누락 / 메모리 손상 / 이중해제로 인한
    // 앨리어싱을 모두 이 함수 하나로 잡아낼 수 있음.
    bool IsCanaryIntact(uint64_t expectedId) const
    {
        if (magic != LIVE_MAGIC) return false;
        if (id != expectedId) return false;
        for (auto p : payload) if (p != expectedId) return false;
        return true;
    }
};

inline std::atomic<int64_t> TestObject::s_ctorCount { 0 };
inline std::atomic<int64_t> TestObject::s_dtorCount { 0 };

// =====================================================================
// 테스트 1: 기본 alloc/free 정합성 + growth-path 생성자 검증
//   - 초기 풀 크기(4)보다 많이(10개) 할당해서, 반드시 growth 경로를
//     한 번 이상 타도록 강제함
//   - 각 객체의 주소 중복 여부(앨리어싱), 생성자 실행 여부(canary)를 확인
// =====================================================================
inline void Test_BasicAllocFree(bool placementNew)
{
    std::cout << "[Test_BasicAllocFree] placementNew=" << placementNew << "\n";

    const int initialPool = 4;
    const int N = 10; // initialPool보다 커야 growth 경로가 반드시 실행됨
    ObjectFreeList<TestObject> pool(initialPool, placementNew);

    std::vector<TestObject*> live;
    live.reserve(N);

    for (int i = 0; i < N; ++i)
    {
        TestObject* obj = pool.Alloc();
        obj->Stamp(1000 + i);
        live.push_back(obj);
    }

    std::set<TestObject*> addrSet(live.begin(), live.end());
    if (addrSet.size() != live.size())
    {
        std::cout << "  [CRITICAL] 중복 주소 발생: 서로 다른 두 개의 살아있는 객체가 "
            "같은 메모리를 가리킵니다 (앨리어싱)\n";
    }

    int failCount = 0;
    for (int i = 0; i < N; ++i)
    {
        bool fromGrowth = (i >= initialPool); // 대략적인 추정치(정확한 내부 카운트는 비공개)
        if (!live[i]->IsCanaryIntact(1000 + i))
        {
            ++failCount;
            std::cout << "  [FAIL] index=" << i
                << (fromGrowth ? " (growth 경로로 추정)" : " (초기 풀에서 추정)")
                << " addr=" << live[i]
                << " magic=0x" << std::hex << live[i]->magic << std::dec
                << " -> 생성자 미실행 또는 메모리 손상\n";
        }
    }
    if (failCount == 0)
        std::cout << "  [OK] " << N << "개 전부 canary 정상\n";

    for (auto* obj : live) pool.Free(obj);
}

// =====================================================================
// __try/__except는 지역에 소멸자 있는 C++ 객체가 없는 함수에서만
// 사용 가능(MSVC C2712)하므로, 실제 Free() 호출은 이 헬퍼로 분리.
// Free()가 __debugbreak()를 유발하는 모든 테스트(이중 해제, 크로스 풀)에서
// 공용으로 씀 - 디버거(F5)로 실행하면 __debugbreak()에서 먼저 멈추므로,
// 이런 테스트들은 Ctrl+F5(디버거 없이 시작)로 실행할 것을 권장.
// =====================================================================
inline int CallFree_NoUnwind(ObjectFreeList<TestObject>* target, TestObject* obj, bool* outReturnValue = nullptr)
{
    __try
    {
        bool result = target->Free(obj);
        if (outReturnValue) *outReturnValue = result;
        return 0; // __debugbreak가 발생하지 않음
    }
    __except (GetExceptionCode() == static_cast<DWORD>(EXCEPTION_BREAKPOINT)
        ? EXCEPTION_EXECUTE_HANDLER
        : EXCEPTION_CONTINUE_SEARCH)
    {
        return 1; // __debugbreak 감지됨
    }
}

// =====================================================================
// 테스트 2: 이중 해제(double-free) 탐지 여부 확인
//   - 지금 ObjectFreeList.h에 이중 해제 방지 로직이 없다면: 두 번째
//     Free()도 그냥 true를 반환하고 조용히 넘어가며, 이후 Alloc() 두
//     번이 같은 주소를 반환하는 것으로 버그가 드러남 -> [CRITICAL]
//   - Node에 inUse 플래그를 추가해 이중 해제 시 __debugbreak()가
//     걸리도록 고쳤다면: 두 번째 Free() 호출에서 바로 감지됨 -> [OK]
//   - 두 경우 모두 크래시 없이 자동 판정되도록 CallFree_NoUnwind로 감쌈.
// =====================================================================
inline void Test_DoubleFree()
{
    std::cout << "[Test_DoubleFree]\n";
    ObjectFreeList<TestObject> pool(4, true);

    TestObject* a = pool.Alloc();
    a->Stamp(42);

    bool ok1 = false;
    CallFree_NoUnwind(&pool, a, &ok1); // 정상적인 첫 번째 해제

    bool ok2 = false;
    int brk2 = CallFree_NoUnwind(&pool, a, &ok2); // 의도적인 이중 해제

    if (brk2)
    {
        std::cout << "  [OK] 이중 해제 시도에서 __debugbreak()가 감지되었습니다 "
            "(이중 해제 방지 로직이 동작 중입니다).\n";
        // 주의: a는 1차 호출에서 이미 정상적으로 풀에 반납되었으므로
        // 여기서 pool.Free(a)를 또 부르면 그 자체가 세 번째 이중 해제 시도가 됨.
        // (SEH로 안 감싼 채 호출하면 진짜로 크래시남 - 절대 다시 추가하지 말 것)
        return;
    }

    std::cout << "  1차 Free() 반환값=" << std::boolalpha << ok1
        << ", 2차(이중) Free() 반환값=" << ok2 << std::noboolalpha << "\n";

    TestObject* b = pool.Alloc();
    TestObject* c = pool.Alloc();

    if (b == c)
    {
        std::cout << "  [CRITICAL] 이중 해제가 감지되지 않았습니다 (Free()가 두 번 다 true 반환). "
            "연속된 Alloc() 두 번이 동일 주소(" << b << ")를 반환했습니다. "
            "즉 서로 다른 두 '살아있는' 포인터가 같은 메모리를 공유하게 됩니다.\n";
    }
    else
    {
        std::cout << "  [정보] 이번 실행에서는 우연히 다른 주소가 나왔습니다 "
            "(freelist 상태에 따라 달라질 수 있음 - 안전하다는 뜻은 아님).\n";
    }

    // 정리(주소가 겹쳤을 수 있으므로 중복 해제하지 않도록 주의)
    pool.Free(b);
    if (b != c) pool.Free(c);
}

// =====================================================================
// 테스트 3: 크로스 풀(cross-pool) 해제 탐지
// =====================================================================
inline void Test_CrossPoolFree()
{
    std::cout << "[Test_CrossPoolFree] (디버거 없이 실행 중이어야 자동 판정됩니다)\n";
    ObjectFreeList<TestObject> poolA(4, true);
    ObjectFreeList<TestObject> poolB(4, true);

    TestObject* obj = poolA.Alloc();
    obj->Stamp(7);

    int detected = CallFree_NoUnwind(&poolB, obj);
    if (detected)
        std::cout << "  [OK] 다른 풀 소유 포인터를 잘못 반납했을 때 __debugbreak()가 감지됨\n";
    else
        std::cout << "  [CRITICAL] 크로스 풀 해제가 감지되지 않았습니다\n";

    poolA.Free(obj); // 올바른 풀로 정리
}

// =====================================================================
// 테스트 4: 동시성 레이스 (현재 구현은 락이 전혀 없음)
//   - 손상이 "이번엔 안 보였다"는 것이 "안전하다"는 뜻이 아님을 강조하기 위한 테스트.
//   - 게임 서버처럼 여러 워커 스레드에서 동시에 쓸 계획이라면 필수로 돌려봐야 함.
// =====================================================================
inline void Test_ConcurrencyRace(int threadCount = 8, int opsPerThread = 200000)
{
    std::cout << "[Test_ConcurrencyRace] threads=" << threadCount
        << " opsPerThread=" << opsPerThread << "\n";

    ObjectFreeList<TestObject> pool(64, true);
    std::atomic<bool> corruptionDetected { false };
    std::atomic<int> corruptionCount { 0 };

    auto worker = [&](int tid)
        {
            std::mt19937_64 rng(static_cast<uint64_t>(tid) + 12345ull);
            for (int i = 0; i < opsPerThread; ++i)
            {
                TestObject* obj = pool.Alloc();
                uint64_t id = (static_cast<uint64_t>(tid) << 32) | static_cast<uint32_t>(i);
                obj->Stamp(id);

                if (!obj->IsCanaryIntact(id))
                {
                    corruptionDetected = true;
                    corruptionCount.fetch_add(1, std::memory_order_relaxed);
                }
                pool.Free(obj);
            }
        };

    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int t = 0; t < threadCount; ++t) threads.emplace_back(worker, t);
    for (auto& th : threads) th.join();

    if (corruptionDetected)
    {
        std::cout << "  [CRITICAL] 동시 접근 중 데이터 손상 " << corruptionCount
            << "건 감지됨 (레이스 컨디션 실증됨)\n";
    }
    else
    {
        std::cout << "  [주의] 이번 실행에서는 손상이 관측되지 않았습니다. "
            "하지만 _head/_countPool 등에 동기화 장치가 전혀 없으므로 "
            "레이스 컨디션 자체는 여전히 존재하며, 타이밍에 따라 재현되지 않을 수 있습니다.\n";
    }
}

// =====================================================================
// 테스트 8: 초기 풀(생성자 warm-up 단계) 이중 생성자 호출 검출
//   - 이번 버전은 초기 풀도 malloc 대신 new Node로 만들면서, growth
//     경로에만 있던 이중 생성 문제가 초기 풀에도 번졌음.
//   - placementNew=false: 풀 생성 시점에 new Node(암묵적 1회) +
//     명시적 placement new(1회) = 즉시 이중 생성.
//   - placementNew=true: 풀 생성 시점에 new Node로 암묵적 1회(낭비),
//     이후 Alloc()으로 실제 꺼낼 때 명시적 placement new로 또 1회 =
//     시점은 떨어져 있지만 총 2회 생성.
// =====================================================================
inline void Test_InitialPoolConstructionCount()
{
    std::cout << "[Test_InitialPoolConstructionCount]\n";

    // Case 1: placementNew = false (eager 모드) - 풀 생성 즉시 확인 가능
    {
        const uint32_t poolSize = 3;
        int64_t before = TestObject::s_ctorCount.load();
        ObjectFreeList<TestObject> pool(poolSize, /*isPlacementNew=*/false);
        int64_t delta = TestObject::s_ctorCount.load() - before;

        std::cout << "  [placementNew=false] 풀 생성(objectCount=" << poolSize
            << ") 직후 생성자 호출 횟수=" << delta;
        if (delta == static_cast<int64_t>(poolSize))
            std::cout << " [OK]\n";
        else
            std::cout << " [CRITICAL] 기대값 " << poolSize << "과 다릅니다. "
            "new Node의 암묵적 생성 + 명시적 placement new가 겹친 것으로 보입니다.\n";
    }

    // Case 2: placementNew = true (지연 생성 모드) - 워밍업 시점과 Alloc() 시점을 나눠서 확인
    {
        int64_t before = TestObject::s_ctorCount.load();
        ObjectFreeList<TestObject> pool(1, /*isPlacementNew=*/true);
        int64_t afterWarmup = TestObject::s_ctorCount.load();

        std::cout << "  [placementNew=true] 풀 생성 직후(Alloc 호출 전) 생성자 호출 횟수="
            << (afterWarmup - before)
            << " (지연 생성이 설계 의도라면 0이어야 하는데, new Node를 쓰는 이상 "
            "여기서 이미 1회 발생합니다)\n";

        TestObject* obj = pool.Alloc(); // 방금 워밍업된 그 노드를 그대로 재사용
        int64_t totalDelta = TestObject::s_ctorCount.load() - before;

        std::cout << "    Alloc() 1회 호출 후 누적 생성자 호출 횟수=" << totalDelta;
        if (totalDelta == 1)
            std::cout << " [OK]\n";
        else
            std::cout << " [CRITICAL] 기대값 1과 다릅니다 (이중 생성 - canary로는 못 잡음).\n";

        pool.Free(obj);
    }
}

// =====================================================================
// 테스트 9: 소멸자 회귀 - 미사용 free-list 노드가 풀 소멸 시 잘못 소멸되는지 확인
//   - placementNew=true 모드에서 free list에 남은 노드는 "생성된 적이
//     없거나 이미 소멸된" 상태여야 정상. 소멸자가 delete를 쓰면 Node의
//     암묵적 소멸자가 T::~T()를 또(또는 처음이자 유일하게, 잘못) 호출함.
//   - 아무것도 Alloc()하지 않은 풀을 그대로 파괴해서, dtor 카운트가
//     0이어야 하는데 실제로 늘어나는지 즉시 확인 (24시간 안 기다려도 됨).
// =====================================================================
inline void Test_TeardownDoubleDestruct()
{
    std::cout << "[Test_TeardownDoubleDestruct] (미사용 노드가 풀 소멸 시 잘못 소멸되는지)\n";

    int64_t dtorBefore = TestObject::s_dtorCount.load();
    {
        // placementNew=true: 이 5개는 애초에 한 번도 생성된 적이 없어야 정상
        ObjectFreeList<TestObject> pool(5, /*isPlacementNew=*/true);
        (void)pool;
    } // 여기서 ~ObjectFreeList() 실행

    int64_t dtorDelta = TestObject::s_dtorCount.load() - dtorBefore;

    std::cout << "  Alloc() 한 번도 안 한 5개짜리 풀을 그대로 파괴 -> 소멸자 호출 횟수="
        << dtorDelta;
    if (dtorDelta == 0)
    {
        std::cout << " [OK]\n";
    }
    else
    {
        std::cout << " [CRITICAL] 생성된 적 없는 객체(또는 이미 소멸된 객체)에 소멸자가 "
            "호출되었습니다. ~ObjectFreeList()의 placementNew=true 분기가 "
            "delete deleteNode를 써서 Node의 암묵적 소멸자(-> T::~T() 포함)를 "
            "부르기 때문입니다. ::operator delete(deleteNode)로 바꾸고 "
            "명시적 ~T() 호출은 생략해야 합니다.\n";
    }
}

// =====================================================================
// 테스트 5: growth 경로 이중 생성자 호출 검출
//   - Node를 new로 만들면(사용자 정의 생성자가 없는 Node는 암묵적으로
//     멤버 T data를 기본 생성함) placementNew=true 분기의 명시적
//     placement new와 합쳐져 T가 총 2번 생성됨.
//   - TestObject는 두 번 생성돼도 최종 상태가 똑같아서(LIVE_MAGIC으로
//     덮어써짐) canary 검사로는 절대 못 잡음 - 생성자 호출 횟수를
//     직접 세는 것만이 유일한 탐지 수단.
// =====================================================================
inline void Test_GrowthPathConstructionCount()
{
    std::cout << "[Test_GrowthPathConstructionCount] (growth 1회당 생성자 호출 횟수 확인)\n";

    const int initialPool = 2;
    ObjectFreeList<TestObject> pool(initialPool, /*isPlacementNew=*/true);

    // 초기 풀을 완전히 소진시켜 다음 Alloc()이 반드시 growth 경로를 타게 함
    std::vector<TestObject*> warm;
    for (int i = 0; i < initialPool; ++i) warm.push_back(pool.Alloc());

    int64_t ctorBefore = TestObject::s_ctorCount.load();

    TestObject* grown = pool.Alloc(); // 여기서부터는 무조건 growth 경로

    int64_t ctorDelta = TestObject::s_ctorCount.load() - ctorBefore;

    if (ctorDelta == 1)
    {
        std::cout << "  [OK] growth 할당 1회당 생성자 1회 호출 확인\n";
    }
    else
    {
        std::cout << "  [CRITICAL] growth 할당 1회에 생성자가 " << ctorDelta
            << "번 호출되었습니다 (기대값 1). Node를 new로 생성하면서 T가 "
            "암묵적으로 한 번 생성되고, placementNew 분기에서 또 생성되어 "
            "이중 생성이 발생한 것으로 보입니다. canary 값(magic/id)은 두 번째 "
            "생성으로 덮어써져 정상으로 보이므로, 이 카운터 기반 검사가 아니면 "
            "이 문제를 발견할 수 없습니다.\n";
    }

    pool.Free(grown);
    for (auto* obj : warm) pool.Free(obj);
}

// =====================================================================
// 테스트 6: #pragma pack(1)로 인한 T 정렬 붕괴 검출
//   - Node에 pack(1)이 걸리면 T data 멤버가 자연 정렬 경계가 아닌
//     임의의 바이트 오프셋에 놓일 수 있음.
//   - x86/x64는 misaligned access를 하드웨어가 허용해서 조용히 넘어갈
//     수 있지만, 여전히 UB이고 이식성이 없으며 성능 손실이 생김.
// =====================================================================
inline void Test_AlignmentCheck()
{
    std::cout << "[Test_AlignmentCheck] (#pragma pack(1) 적용 이후 T 정렬 확인)\n";

    constexpr size_t requiredAlign = alignof(TestObject);
    std::cout << "  TestObject가 원래 요구하는 정렬: " << requiredAlign << "바이트\n";

    ObjectFreeList<TestObject> pool(2, true);

    std::vector<TestObject*> objs;
    for (int i = 0; i < 4; ++i) objs.push_back(pool.Alloc()); // 초기 풀 2개 + growth 2개

    int misaligned = 0;
    for (size_t i = 0; i < objs.size(); ++i)
    {
        uintptr_t addr = reinterpret_cast<uintptr_t>(objs[i]);
        if (addr % requiredAlign != 0)
        {
            ++misaligned;
            std::cout << "  [WARN] index=" << i << " addr=" << objs[i]
                << " -> " << (addr % requiredAlign) << "바이트만큼 정렬이 어긋남\n";
        }
    }

    if (misaligned == 0)
        std::cout << "  [정보] 이번 실행에서는 우연히 전부 정렬이 맞았습니다. "
        "다만 pack(1)이 정렬 요구를 아예 무시하도록 강제하므로, T의 "
        "필드 구성이나 컴파일러/아키텍처가 바뀌면 언제든 깨질 수 있는 상태입니다.\n";
    else
        std::cout << "  [CRITICAL] " << misaligned << "개 객체가 정렬 요구사항을 위반했습니다.\n";

    for (auto* obj : objs) pool.Free(obj);
}

// =====================================================================
// 테스트 7: malloc + delete 불일치 안내
//   - 초기 풀 노드는 std::malloc()으로 만들지만 소멸자는 delete로 해제함.
//   - 이건 UB라서 일반 Release 빌드에서는 크래시 없이 조용히 넘어갈 수
//     있음 - 프로그램적으로 100% 자동 판정은 어렵고, ASan이 가장 확실한
//     검출 수단이라 여기서는 재현 코드 + 안내 메시지만 제공.
// =====================================================================
inline void Test_MismatchedAllocatorNote()
{
    std::cout << "[Test_MismatchedAllocatorNote]\n"
        "  이번 버전은 초기 풀도 growth 경로와 동일하게 new Node로 만들고 "
        "소멸자도 delete를 쓰므로, malloc/delete 불일치 문제는 해결된 것으로 "
        "보입니다. 그래도 회귀 확인용으로 재현 블록은 남겨둡니다 - ASan으로 "
        "돌렸을 때 'alloc-dealloc-mismatch'가 더 이상 뜨지 않아야 정상입니다. "
        "(대신 이중 생성 문제는 Test_InitialPoolConstructionCount / "
        "Test_GrowthPathConstructionCount 에서 확인하세요.)\n";

    {
        ObjectFreeList<TestObject> pool(4, true);
        TestObject* obj = pool.Alloc();
        obj->Stamp(1);
        pool.Free(obj);
    } // <- ~ObjectFreeList()가 여기서 new'd 노드를 delete로 해제함 (일관성 있음)

    std::cout << "  (ASan으로 이 지점에서 alloc-dealloc-mismatch가 뜨면 아직 어딘가 "
        "malloc/new가 섞여있다는 뜻입니다.)\n";
}

// 위 7가지 빠른 테스트를 순서대로 실행 (수 초 내 종료, 설계 결함을 즉시 드러냄)
// 아래는 TestConfig.h에서 정의됩니다. TestConfig.h 없이 이 헤더만
// 단독으로 쓸 경우를 대비해 기본값(전부 실행)을 fallback으로 둡니다.
#ifndef RUN_TEST_BASIC_ALLOC_FREE
#define RUN_TEST_BASIC_ALLOC_FREE 1
#endif
#ifndef RUN_TEST_DOUBLE_FREE
#define RUN_TEST_DOUBLE_FREE 1
#endif
#ifndef RUN_TEST_CROSS_POOL_FREE
#define RUN_TEST_CROSS_POOL_FREE 1
#endif
#ifndef RUN_TEST_CONCURRENCY_RACE
#define RUN_TEST_CONCURRENCY_RACE 1
#endif
#ifndef CONCURRENCY_THREAD_COUNT
#define CONCURRENCY_THREAD_COUNT 8
#endif
#ifndef CONCURRENCY_OPS_PER_THREAD
#define CONCURRENCY_OPS_PER_THREAD 200000
#endif
#ifndef RUN_TEST_GROWTH_CTOR_COUNT
#define RUN_TEST_GROWTH_CTOR_COUNT 1
#endif
#ifndef RUN_TEST_ALIGNMENT_CHECK
#define RUN_TEST_ALIGNMENT_CHECK 1
#endif
#ifndef RUN_TEST_MISMATCHED_ALLOC
#define RUN_TEST_MISMATCHED_ALLOC 1
#endif
#ifndef RUN_TEST_INITIAL_POOL_CTOR_COUNT
#define RUN_TEST_INITIAL_POOL_CTOR_COUNT 1
#endif
#ifndef RUN_TEST_TEARDOWN_DOUBLE_DESTRUCT
#define RUN_TEST_TEARDOWN_DOUBLE_DESTRUCT 1
#endif

inline void RunQuickCorrectnessTests()
{
    std::cout << "========== 빠른 정합성 테스트 시작 ==========\n";

#if RUN_TEST_BASIC_ALLOC_FREE
    Test_BasicAllocFree(true);
    Test_BasicAllocFree(false);
#endif

#if RUN_TEST_DOUBLE_FREE
    Test_DoubleFree();
#endif

#if RUN_TEST_CROSS_POOL_FREE
    Test_CrossPoolFree();
#endif

#if RUN_TEST_CONCURRENCY_RACE
    Test_ConcurrencyRace(CONCURRENCY_THREAD_COUNT, CONCURRENCY_OPS_PER_THREAD);
#endif

#if RUN_TEST_GROWTH_CTOR_COUNT
    Test_GrowthPathConstructionCount();
#endif

#if RUN_TEST_ALIGNMENT_CHECK
    Test_AlignmentCheck();
#endif

#if RUN_TEST_MISMATCHED_ALLOC
    Test_MismatchedAllocatorNote();
#endif

#if RUN_TEST_INITIAL_POOL_CTOR_COUNT
    Test_InitialPoolConstructionCount();
#endif

#if RUN_TEST_TEARDOWN_DOUBLE_DESTRUCT
    Test_TeardownDoubleDestruct();
#endif

    std::cout << "========== 빠른 정합성 테스트 종료 ==========\n\n";
}

// =====================================================================
// 장기 소크 테스트 (기본 24시간)
//   - 무작위 alloc/free churn을 반복하며, 살아있는 모든 객체의 canary를
//     주기적으로 전수 검사
//   - 60초마다 진행 상황을 로그 파일에 기록 (하루 종일 지켜보지 않아도 됨)
//   - 종료 시 ctor/dtor 카운트 정합성도 함께 검증
// =====================================================================
inline void RunSoakTest(std::chrono::seconds duration, bool placementNew, std::ostream& log)
{
    ObjectFreeList<TestObject> pool(256, placementNew);
    std::mt19937_64 rng(std::random_device {}());
    std::uniform_real_distribution<double> coin(0.0, 1.0);

    std::vector<TestObject*> live;
    std::unordered_map<TestObject*, uint64_t> expectedId;
    live.reserve(200000);

    uint64_t totalAllocs = 0, totalFrees = 0, peakLive = 0, corruptionEvents = 0;
    uint64_t nextId = 1;

    int64_t ctorBefore = TestObject::s_ctorCount.load();
    int64_t dtorBefore = TestObject::s_dtorCount.load();

    auto start = std::chrono::steady_clock::now();
    auto lastReport = start;
    auto lastSweep = start;

    while (std::chrono::steady_clock::now() - start < duration)
    {
        bool doAlloc = (live.size() < 1000000) && (coin(rng) < 0.6);

        if (doAlloc || live.empty())
        {
            TestObject* obj = pool.Alloc();
            uint64_t id = nextId++;
            obj->Stamp(id);
            live.push_back(obj);
            expectedId[obj] = id;
            ++totalAllocs;
            peakLive = (std::max)(peakLive, static_cast<uint64_t>(live.size()));
        }
        else
        {
            std::uniform_int_distribution<size_t> pick(0, live.size() - 1);
            size_t idx = pick(rng);
            TestObject* obj = live[idx];

            if (!obj->IsCanaryIntact(expectedId[obj]))
            {
                ++corruptionEvents;
                log << "[CORRUPTION] addr=" << obj
                    << " expected=" << expectedId[obj]
                    << " magic=0x" << std::hex << obj->magic << std::dec << "\n";
            }

            bool freed = pool.Free(obj);
            if (!freed)
            {
                ++corruptionEvents;
                log << "[FREE-FAILED] addr=" << obj
                    << " Free()가 false를 반환했습니다 (쿠키 손상 가능성)\n";
            }
            expectedId.erase(obj);
            live[idx] = live.back();
            live.pop_back();
            ++totalFrees;
        }

        auto now = std::chrono::steady_clock::now();
        if (now - lastSweep > std::chrono::seconds(10))
        {
            for (auto* obj : live)
            {
                if (!obj->IsCanaryIntact(expectedId[obj]))
                {
                    ++corruptionEvents;
                    log << "[CORRUPTION-SWEEP] addr=" << obj << "\n";
                }
            }
            lastSweep = now;
        }

        if (now - lastReport > std::chrono::seconds(60))
        {
            auto elapsedMin = std::chrono::duration_cast<std::chrono::minutes>(now - start).count();
            log << "[진행] " << elapsedMin << "분 경과 | alloc=" << totalAllocs
                << " free=" << totalFrees << " live=" << live.size()
                << " peakLive=" << peakLive << " corruption=" << corruptionEvents
                << std::endl;
            lastReport = now;
        }
    }

    for (auto* obj : live) pool.Free(obj);

    int64_t ctorDelta = TestObject::s_ctorCount.load() - ctorBefore;
    int64_t dtorDelta = TestObject::s_dtorCount.load() - dtorBefore;

    log << "\n===== 소크 테스트 종료 (placementNew=" << placementNew << ") =====\n"
        << "총 alloc: " << totalAllocs << "\n"
        << "총 free : " << totalFrees << "\n"
        << "최대 동시 보유: " << peakLive << "\n"
        << "손상 감지 횟수: " << corruptionEvents << "\n"
        << "ctor 호출 증가량: " << ctorDelta << "\n"
        << "dtor 호출 증가량: " << dtorDelta << "\n";

    if (placementNew)
    {
        // placementNew=true 모드: Alloc마다 생성, Free마다 소멸이 원칙이므로
        // 최종적으로 ctor 증가량과 dtor 증가량이 같아야 함(모든 객체를 반납했으므로).
        log << (ctorDelta == dtorDelta
            ? "ctor/dtor 정합성: OK\n"
            : "ctor/dtor 정합성: [FAIL] 생성/소멸 횟수 불일치 - 초기 풀/growth 경로의 "
            "이중 생성 문제와 연관 가능성 높음\n");
    }

    log << (corruptionEvents == 0 ? "최종 결과: PASS (손상 없음)\n"
        : "최종 결과: FAIL (손상 발생, 위 로그 확인)\n");
}
