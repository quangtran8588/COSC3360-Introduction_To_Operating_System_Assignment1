#pragma once
#include "InputList.h"
#include <string>

using namespace std;

struct Block
{
	InputList list;
	InputList *current;
	InputList *topList;
	Block *next;
	Block *prev;
	Block* setDataBlock(string operation, int meter, int processNo, bool empty, Block *block);
	string returnBlock(Block *block);
};

class InputTable
{
private:
	Block *ptr;
	Block *topBlock;
	bool empty;
	int processNo;
public:
	InputTable();
	~InputTable();
	InputTable* setTable(string operation, int meter, InputTable *table);
	InputTable* updateTable(InputTable* table,InputList* input, int meter, bool doneProcess);
	InputList* searchTable(InputTable* table, int &startTime);
	void clean(InputTable* table);
};

