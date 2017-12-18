#include "InputList.h"

InputList::InputList()
{
	processID = 0;
	operation = "";
	meter = 0;
	bool doneProcess = false;
	inProcess = false;
	next = NULL;
	prev = NULL;
}

InputList::~InputList()
{
}


InputList* InputList::setData(int ID, string opr,int met, InputList *current, bool empty, InputList *newList)
{
	newList->processID = ID;
	newList->operation = opr;
	newList->meter = met;
	if (empty) { newList->prev = NULL, newList->next = NULL; }
	else 
	{
		newList->prev = current;
		newList->next = NULL;
		current->next = newList;
	}
	return newList;
}

InputList* InputList::updateData(InputList* input, int met, bool done)
{
	input->meter = met;
	input->doneProcess = done;
	return input;
}

InputList* InputList::returnActiveProcess(InputList* input)
{
	if (input->operation == "NEW")
	{
		input->inProcess = true;
		return input->next;
	}
	else if (input->meter == 0 && input->doneProcess)
		return input->next;
	return NULL;	
}

int InputList::getProcessID()
{
	return this->processID;
}

string InputList::getOperation()
{
	return this->operation;
}
int InputList::getMeter()
{
	return this->meter;
}
bool InputList::checkInProcess(InputList* list)
{
	if (!list->inProcess)
		return false;
	return true;
}

void InputList::clean(InputList* list)
{
	do
	{
		InputList* temp = list;
		if (list->next != NULL)
		{
			list->next->prev = NULL;
			list = list->next;
			delete temp;
		}
		else
		{
			delete temp;
			break;
		}
	} while (1);
}

string InputList::toString(InputList *list)
{
	string toString;
	do
	{
		toString += list->getOperation() + " " + to_string(list->getMeter()) + "\n";
		if (list->next != NULL) list = list->next;
		else break;
	} while (1);
	return toString;
}
