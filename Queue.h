#pragma once
#include "ProcessTable.h"
#include "InputList.h"
#include <string>
#include <iostream>

using namespace std;

struct queueList
{
	List* list;
	queueList* next;
	queueList* prev;
};

class Queue
{
private:
	List* list;
	queueList* frontQueue;
	int counter;
public:
	Queue();
	~Queue();
	void push(List* newList);
	List* front();
	void pop();
	bool exist(List* newList);
	bool empty();
	int size();
	void print(Queue* list);
};

