#pragma once
#include <string>
#include "InputList.h"
using namespace std;

class Core
{
private:
	string status;
	int exeTime;
	InputList* currentProcess;
public:
	Core();
	~Core();
	int getExeTime();
	string getStatus();
	InputList* getCurrentProcess();
	void setExeTime(int newExeTime);
	void setStatus(string newStatus);
	void setCurrentProcess(InputList* input);
	bool checkIdle(int sysTime);
};

