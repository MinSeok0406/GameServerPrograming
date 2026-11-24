#pragma once
#include <Windows.h>

class CpuUsage
{
public:
	//---------------------------------------------------------------------- 
	// 생성자, 확인대상 프로세스 핸들. 미입력시 자기 자신. 
	//---------------------------------------------------------------------- 
	CpuUsage(HANDLE hProcess = INVALID_HANDLE_VALUE);

	void updateCpuTime();

	float processorTotal() { return _fProcessorTotal; }
	float processorUser() { return _fProcessorUser; }
	float processorKernel() { return _fProcessorKernel; }

	float processTotal() { return _fProcessTotal; }
	float processUser() { return _fProcessUser; }
	float processKernel() { return _fProcessKernel; }

private:
	HANDLE _hProcess;
	int _iNumberOfProcessors;

	float _fProcessorTotal;
	float _fProcessorUser;
	float _fProcessorKernel;

	float _fProcessTotal;
	float _fProcessUser;
	float _fProcessKernel;

	ULARGE_INTEGER _ftProcessor_LastKernel;
	ULARGE_INTEGER _ftProcessor_LastUser;
	ULARGE_INTEGER _ftProcessor_LastIdle;

	ULARGE_INTEGER _ftProcess_LastKernel;
	ULARGE_INTEGER _ftProcess_LastUser;
	ULARGE_INTEGER _ftProcess_LastTime;
};