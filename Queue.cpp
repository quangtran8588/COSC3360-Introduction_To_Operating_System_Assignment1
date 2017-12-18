#include "Queue.h"

Queue::Queue()
{
	frontQueue = NULL;
	counter = 0;
}


Queue::~Queue()
{
}

void Queue::push(List* newList)
{
	queueList *newQueue = new queueList();
	newQueue->list = newList;
	if (counter == 0)
	{
		newQueue->next = NULL;
		newQueue->prev = NULL;
		this->frontQueue = newQueue;
		counter++;
	}
	else
	{
		counter++;
		queueList *temp = frontQueue;
		do
		{
			if (temp->next != NULL) temp = temp->next;
			else
			{
				temp->next = newQueue;
				newQueue->prev = temp;
				newQueue->next = NULL;
				break;
			}
		} while (1);
	}
}
List* Queue::front()
{
	return this->frontQueue->list;
}
void Queue::pop()
{
	queueList* temp;
	temp = frontQueue;
	counter--;
	if (counter == 0)
		frontQueue = NULL;
	else
	{
		temp->next->prev = NULL;
		this->frontQueue = temp->next;
	}
}
bool Queue::exist(List *newList)
{
	queueList* temp = frontQueue;
	do
	{
		if (temp->list == newList) return true;
		else
		{
			if (temp->next != NULL) 
				temp = temp->next;
			else
				return false;
		}
	} while (1);
}
bool Queue::empty()
{
	if (counter == 0)
		return true;
	return false;
}
int Queue::size()
{
	return this->counter;
}

void Queue::print(Queue* list)
{
	InputList* input;
	List* temp;
	queueList* queue;
	queue = list->frontQueue;
	cout << "Process\t\t" << "Operation\t" << "Meter" << endl;
	do
	{
		cout << queue->list->processID << "\t\t" << queue->list->currentProcess->getOperation() << "\t\t" << queue->list->currentProcess->getMeter() << endl;
		if (queue->next != NULL)
			queue = queue->next;
		else
			break;
	} while (1);
	cout << "\n";
}