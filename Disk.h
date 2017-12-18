#pragma once
#include <string>
#include "InputList.h"

using namespace std;

class Disk
{
private:
	string status = "Free";
	int exeTime = 0;
	InputList* currentProcess;
public:
	Disk();
	~Disk();
	int getExeTime();
	string getStatus();
	InputList* getCurrentProcess();
	void setExeTime(int newExeTime);
	void setStatus(string newStatus);
	void setCurrentProcess(InputList* input);
	bool checkIdle(int sysTime);
};


