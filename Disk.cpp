#include "Disk.h"


Disk::Disk()
{
	string status = "Free";
	int exeTime = 0;
}

Disk::~Disk()
{
}

int Disk::getExeTime()
{
	return this->exeTime;
}

string Disk::getStatus()
{
	return this->status;
}

InputList* Disk::getCurrentProcess()
{
	return this->currentProcess;
}
void Disk::setExeTime(int newExeTime)
{
	exeTime = newExeTime;
}

void Disk::setStatus(string newStatus)
{
	status = newStatus;
}
void Disk::setCurrentProcess(InputList* input)
{
	currentProcess = input;
}
bool Disk::checkIdle(int sysTime)
{
	if (status == "In Process" && sysTime < exeTime) return false;
	else if (status == "In Process" && sysTime >= exeTime)
	{
		status = "Free";
		return true;
	}
	return true;
}
