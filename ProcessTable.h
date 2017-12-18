#pragma once
#include <string>
#include <limits.h>
#include "InputList.h"

struct List
{
	InputList* currentProcess;
	int startTime;
	int endTime;
	int processID;
	string status;
	List* next;
	List* prev;
	List() : currentProcess(NULL), startTime(0), endTime(0), processID(0), status(""), next(NULL) {}
};

using namespace std;
class ProcessTable
{
private:
	List* top;
	List* ptr;
	bool empty;
	int count;
public:
	ProcessTable();
	~ProcessTable();
	ProcessTable* push(ProcessTable* table,int start, int end, int ID, InputList* current, string status);
	ProcessTable* pop(ProcessTable* table);
	ProcessTable* update(ProcessTable* table, int end,int ID, InputList* current, string status);
	List* getList(ProcessTable* table);
	List* searchList(ProcessTable* table, int ID);
	bool exist(ProcessTable* table,List* list);
	int minEndTime(ProcessTable* table);
	bool complete(ProcessTable* table);
};

