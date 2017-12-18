#include "InputTable.h"
/*****************************************************************************
	-	Input Table will contain Blocks which represent each process with process ID
	-	Each block will contain many events which need to be processed
*****************************************************************************/
Block* Block::setDataBlock(string operation, int meter, int processNo, bool empty, Block *block)
{
	InputList *newInput = new InputList();
	if (empty)
	{
		newInput = newInput->setData(processNo, operation, meter, newInput, true, newInput);
		block->topList = newInput;
		block->current = newInput;
	}
	else
	{
		newInput = newInput->setData(processNo, operation, meter, block->current, false, newInput);
		block->current = newInput;
	}
	return block;
}

string Block::returnBlock(Block *block)
{
	block->current = topList;
	string toString;
	toString = block->current->toString(block->current);
	return toString;
}

InputTable::InputTable()
{
	ptr = NULL;
	topBlock = NULL;
	empty = true;
	processNo = -1;
}

InputTable::~InputTable()
{
}

InputTable* InputTable::setTable(string operation, int meter, InputTable *table)
{
	if (operation == "NEW")
	{
		Block *newBlock;
		processNo++;
		if (table->empty)
		{
			newBlock = new Block();
			table->empty = false;
			newBlock = newBlock->setDataBlock(operation, meter, processNo, true, newBlock);
			table->topBlock = newBlock;
			table->ptr = newBlock;
			newBlock->prev = newBlock->next = NULL;
		}
		else
		{
			newBlock = new Block();
			newBlock = newBlock->setDataBlock(operation, meter, processNo, true, newBlock);
			table->ptr->next = newBlock;
			newBlock->prev = table->ptr;
			newBlock->next = NULL;
			table->ptr = newBlock;
		}
	}
	else
	{
		table->ptr->setDataBlock(operation, meter, processNo, false, table->ptr);
	}
	return table;
}

InputTable* InputTable::updateTable(InputTable* table,InputList* input, int meter, bool doneProcess)
{
	input = input->updateData(input, meter, doneProcess);
	if (meter == 0 && doneProcess)
		input = input->returnActiveProcess(input);
	return table;
}

InputList* InputTable::searchTable(InputTable* table, int &startTime)
{
	Block* block;
	InputList* list;
	block = table->topBlock;
	list = block->topList;
	do
	{
		startTime = list->getMeter();
		if (!list->checkInProcess(list))
		{
			list = list->returnActiveProcess(list);
			if (list != NULL) return list;
		}
		else if (block->next != NULL)
		{
			block = block->next;
			list = block->topList;
		}
		else break; 
	} while (1);
	return NULL;
}

void InputTable::clean(InputTable* table)
{
	Block* tempBlock, *currentBlock;
	InputList* tempList;
	tempBlock = table->topBlock;
	do
	{
		currentBlock = tempBlock;
		tempList = tempBlock->topList;
		tempList->clean(tempList);
		if (tempBlock->next != NULL)
		{
			tempBlock = tempBlock->next;
			delete currentBlock;
		}
		else
		{
			delete currentBlock;
			break;
		}
	} while (1);
}
