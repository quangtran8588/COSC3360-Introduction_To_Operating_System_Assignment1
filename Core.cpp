#include "Core.h"


Core::Core()
{
	status = "Free";
	exeTime = 0;
	currentProcess = NULL;
}

Core::~Core()
{
}

int Core::getExeTime()
{
	return this->exeTime;
}

string Core::getStatus()
{
	return this->status;
}

InputList* Core::getCurrentProcess()
{
	return this->currentProcess;
}

void Core::setExeTime(int newExeTime)
{
	exeTime = newExeTime;
}

void Core::setStatus(string newStatus)
{
	status = newStatus;
}

void Core::setCurrentProcess(InputList* input)
{
	currentProcess = input;
}

bool Core::checkIdle(int sysTime)
{
	if (status == "In Process" && sysTime < exeTime) return false;
	else if (status == "In Process" && sysTime >= exeTime)
	{
		status = "Free";
		return true;
	}
	return true;
}