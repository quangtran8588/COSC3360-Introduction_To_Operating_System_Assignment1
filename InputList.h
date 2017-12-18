#pragma once
#include <string>

using namespace std;

class InputList
{
private:
	int processID;
	string operation;
	int meter;
	bool doneProcess;
	bool inProcess;
	InputList *next;
	InputList *prev;
public:
	InputList();
	~InputList();
	InputList* setData(int ID, string opr, int met,InputList *current, bool empty, InputList *newList);
	InputList* updateData(InputList* input, int met, bool done);
	InputList* returnActiveProcess(InputList* input);
	int getProcessID();
	string getOperation();
	int getMeter();
	bool checkInProcess(InputList* list);
	void clean(InputList* list);
	string toString(InputList *list);
};

