#include "ProcessTable.h"

/*******************************************************************************
	- Process Table contains all processes that need to be executed
	- Each list in Process Table will contain pointer which point to the top 
		current event of each process in Input Table
********************************************************************************/

ProcessTable::ProcessTable()
{
	top = NULL;
	ptr = NULL;
	empty = true;
	count = 0;
}

ProcessTable::~ProcessTable()
{
}

ProcessTable* ProcessTable::push(ProcessTable* table, int start, int end, int ID, InputList* current, string status)
{
	List* newList;
	count++;
	if (empty)
	{
		empty = false;
		newList = new List();
		newList->startTime = start;		
		newList->endTime = end;		newList->processID = ID;
		newList->currentProcess = current;
		newList->status = status;
		newList->next = NULL; newList->prev = NULL;
		table->ptr = table->top = newList;
	}
	else
	{
		newList = new List();
		newList->startTime = start;	
		newList->endTime = end;		newList->processID = ID;
		newList->currentProcess = current;
		newList->status = status;
		table->ptr->next = newList;
		newList->next = NULL; newList->prev = table->ptr;
		table->ptr = newList;
	}
	return table;
}

List* ProcessTable::searchList(ProcessTable* table, int ID)
{
	List* list;
	list = table->top;
	while (list->processID != ID)
	{	list = list->next;	}
	return list;
}

ProcessTable* ProcessTable::update(ProcessTable* table, int end,int ID, InputList* current, string status)
{
	List* list;
	list = table->searchList(table, ID);
	list->endTime = end;
	list->currentProcess = current;
	list->status = status;
	return table;
}
List* ProcessTable::getList(ProcessTable* table)
{
	List* newList;
	newList = table->top;
	return newList;
}

ProcessTable* ProcessTable::pop(ProcessTable* table)
{
	List* list;
	count--;
	list = table->top;
	while (list->status != "TERMINATED")
	{
		list = list->next;
	}
	if (list->next == NULL && count != 0)
	{
		list->prev->next = NULL;
		table->ptr = list->prev;
	}
	else if (list->prev == NULL && count != 0)
	{
		list->next->prev = NULL;
		table->top = list->next;
	}
	else if (list->prev != NULL && list->next != NULL)
	{
		list->next->prev = list->prev;
		list->prev->next = list->next;
	}
	else if (count == 0)
	{
		table->ptr = table->top = NULL;
		empty = true;
	}
	delete list;
	return table;
}
bool ProcessTable::exist(ProcessTable* table, List* list)
{
	if (table->count == 0) return false;
	List* temp = table->top;
	do
	{
		if (temp->currentProcess == list->currentProcess) return true;
		if (temp->next != NULL) temp = temp->next;
		else break;
	} while (1);
	return false;
}
int ProcessTable::minEndTime(ProcessTable* table)
{
	int min = INT_MAX;
	List* list = table->top;
	do
	{
		if (list->endTime < min) min = list->endTime;
		if (list->next != NULL) list = list->next;
		else break;
	} while (1);
	return min;
}

bool ProcessTable::complete(ProcessTable* table)
{
	if (table->count == 0)
		return true;
	return false;
}
