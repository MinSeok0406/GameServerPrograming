// 빌드: Visual Studio 콘솔 앱 프로젝트에 이 4개 파일을 추가
//   ObjectFreeList.h / FreeListValidator.h / TestConfig.h / main.cpp
//
// 실행 방법 변경 없음: TestConfig.h의 값을 원하는 대로 바꾸고
// 다시 빌드/실행하면 됩니다. 콘솔 인자(argv)는 사용하지 않습니다.
//
//   예) 빠른 테스트만 보고 싶다  -> TestConfig.h에서 RUN_SOAK_TEST 를 0으로
//   예) 3분만 짧게 소크 테스트   -> TestConfig.h에서 SOAK_HOURS 를 0.05 로
//   예) eager 생성 모드도 확인   -> TestConfig.h에서 SOAK_PLACEMENT_NEW 를 false 로
//
// 주의: Test_CrossPoolFree / Test_DoubleFree 는 __debugbreak()를 유발합니다.
//       Visual Studio에서 F5(디버거 포함 실행)로 돌리면 그 지점에서 멈추므로,
//       Ctrl+F5(디버거 없이 시작)로 실행하는 것을 권장합니다.

#include "TestConfig.h"        // FreeListValidator.h보다 반드시 먼저 include
#include "FreeListValidator.h"
#include <fstream>

int main()
{
#if RUN_QUICK_TESTS
    RunQuickCorrectnessTests();
#endif

#if RUN_SOAK_TEST
    std::ofstream log(SOAK_LOG_FILE, std::ios::app);
    log << "\n===== 소크 테스트 시작 (placementNew=" << SOAK_PLACEMENT_NEW
        << ", 목표 시간=" << SOAK_HOURS << "시간) =====" << std::endl;

    std::cout << "\n소크 테스트를 " << SOAK_HOURS << "시간 동안 실행합니다. "
        << "진행 상황은 " << SOAK_LOG_FILE << " 에 1분 간격으로 기록됩니다.\n";

    RunSoakTest(std::chrono::seconds(static_cast<long long>(SOAK_HOURS * 3600.0)),
        SOAK_PLACEMENT_NEW, log);

    std::cout << "\n소크 테스트 완료. " << SOAK_LOG_FILE
        << " 마지막 부분에서 '최종 결과: PASS/FAIL'을 확인하세요.\n";
#else
    std::cout << "\nRUN_SOAK_TEST가 0이므로 소크 테스트는 건너뜁니다. "
        "빠른 테스트 결과만 확인하세요.\n";
#endif

    return 0;
}
