#pragma once

// ============================================================
//  테스트 실행 설정
//  이 파일의 값들만 바꿔서 다시 빌드하면 됩니다. 콘솔 인자(argv)는
//  필요 없습니다 — 모든 것이 컴파일 타임에 결정됩니다.
//
//  주의: main.cpp에서 이 헤더를 FreeListValidator.h보다
//        "먼저" #include 해야 아래 값들이 제대로 반영됩니다.
// ============================================================

// ---------------- 최상위 스위치 (0 또는 1) ----------------
#define RUN_QUICK_TESTS             0   // 빠른 정합성 테스트 (수 초 내 종료)
#define RUN_SOAK_TEST               1   // 장기 소크 테스트

// ---------------- 빠른 테스트 개별 on/off (0 또는 1) ----------------
#define RUN_TEST_BASIC_ALLOC_FREE   1   // growth-path 생성자 누락 버그 검출
#define RUN_TEST_DOUBLE_FREE        1   // 이중 해제 감지 여부 검증
#define RUN_TEST_CROSS_POOL_FREE    1   // 크로스 풀 해제 감지(__debugbreak) 검증
#define RUN_TEST_CONCURRENCY_RACE   0   // 락 없는 동시 접근 레이스 재현 시도
#define RUN_TEST_GROWTH_CTOR_COUNT  1   // growth 경로 이중 생성자 호출 검출 (canary로는 못 잡음)
#define RUN_TEST_ALIGNMENT_CHECK    1   // #pragma pack(1)로 인한 T 정렬 붕괴 검출
#define RUN_TEST_MISMATCHED_ALLOC   1   // malloc+delete 불일치 회귀 확인
#define RUN_TEST_INITIAL_POOL_CTOR_COUNT 1  // 초기 풀 이중 생성 검출
#define RUN_TEST_TEARDOWN_DOUBLE_DESTRUCT 1 // 소멸자의 미사용 노드 이중 소멸 검출

// ---------------- 동시성 테스트 파라미터 ----------------
#define CONCURRENCY_THREAD_COUNT    8
#define CONCURRENCY_OPS_PER_THREAD  200000

// ---------------- 소크 테스트 설정 ----------------
// 시간 단위(시간). 예: 24.0 = 24시간, 0.05 = 3분(짧게 먼저 확인해볼 때)
#define SOAK_HOURS                  10

// true: placement-new 모드(Alloc마다 생성/Free마다 소멸)
// false: eager 생성 모드(풀 생성 시 한 번만 생성, 이후 재사용)
// 실사용 방식에 맞는 값으로 두고, 필요하면 두 값 다 한 번씩 돌려보세요.
#define SOAK_PLACEMENT_NEW          false

#define SOAK_LOG_FILE               "soak_log_false.txt"
