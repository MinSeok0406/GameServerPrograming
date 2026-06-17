#include <iostream>
#include <Windows.h>
#include "RoundRobin.h"
using namespace std;

class Process
{
public:
	explicit Process(string name) : m_name(move(name))
	{

	}

	void doWorkDuringTimeSlice()
	{
		cout << "Process " << m_name << " performing work during time slice." << "\n";
	}

	bool operator==(const Process&) const = default;

private:
	string m_name;
};

class Scheduler
{
public:
	explicit Scheduler(const vector<Process>& processes)
	{
		for (auto& process : processes)
		{
			m_processes.add(process);
		}
	}

	void scheduleTimeSlice()
	{
		try
		{
			m_processes.getNext().doWorkDuringTimeSlice();
		}
		catch (...)
		{
			cerr << "No processes" << "\n";
		}
	}

	void removeProcess(const Process& process)
	{
		m_processes.remove(process);
	}

private:
	RoundRobin<Process> m_processes;
};

int wmain(int argc, WCHAR* argv[])
{
	vector<Process> v{ Process {"1"}, Process {"2"}, Process {"3"} };

	Scheduler scheduler(v);
	for (auto i = 0; i < 4; ++i)
	{
		scheduler.scheduleTimeSlice();
	}
	
	scheduler.removeProcess(v[2]);
	cout << "Removed Third process" << "\n";

	for (auto i = 0; i < 4; ++i)
	{
		scheduler.scheduleTimeSlice();
	}

	return 0;
}