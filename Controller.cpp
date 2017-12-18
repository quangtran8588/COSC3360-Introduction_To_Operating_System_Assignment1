#include "Controller.h"

InputTable *inputTable = new InputTable();
ProcessTable *processTable = new ProcessTable();
ProcessTable *userRequest = new ProcessTable();
Core *core;
Disk *disk;
int numbOfCore = 0;
int slice = 0;
int sysTime = 0;
Queue readyQueue;
Queue diskQueue;

Controller::Controller()
{
}

Controller::~Controller()
{

}

/************************************************************************************/
/************************************************************************************
	- Create number of Core CPU base on the data in the text file
	- Create Disk
	- Initialize public variable Quantum time
	- Create link lists to store all processes data into Input Table
*************************************************************************************/
//	Function call to create an array of CPU Core class
void createCoreProcess(int numb)
{
	core = new Core[numb];
	for (int i = 0; i < numb; i++)
	{
		core[i] = Core();
	}
}

void Controller::setInputTable(string data1, string data2)
{
	if (data1 == "NCORES") numbOfCore = stoi(data2);
	else if (data1 == "SLICE") slice = stoi(data2);
	else
	{
		inputTable = inputTable->setTable(data1, stoi(data2), inputTable);
	}
	createCoreProcess(numbOfCore);
	disk = new Disk;
}
/***********************************************************************************/

/***********************************************************************************/
/***********************************************************************************
	- Create Process Table of all processes in the Input Table.
	- Process Table will store the top pointer event of each process, arriving time
	- and the process ID to which event belongs
************************************************************************************/
void Controller::createProcessTable()
{
	InputList *newList;
	int start;
	do
	{
		newList = inputTable->searchTable(inputTable, start);
		if (newList != NULL)
			processTable = processTable->push(processTable, start, 0, newList->getProcessID(), newList, "NOT RUNNING");
	} while (newList != NULL);	
}
/***********************************************************************************/

/***********************************************************************************/
/***********************************************************************************
	- Create a loop to perform all processes in the Process Table		
	- There are some private functions that support to execute Process Table
		+ freeCore(): check if any core is free and return. Otherwise, return NULL
		+ freeDisk(): check if Disk is free and return
		+ numbOfBusyCore(): return total number of busy at the moment so that 
			we can pop out the right amount of event in Ready Queue to process
		+ terminate(): print out Process Table when there's one process is terminated
		+ minCoreExeTime(): There are multiple cores running at the same time,
			each of them might end at different time. So, function call to return
			the minimum end time to compare with other execution time (disk, user request)
			so that we can update the current time of system
		+ doneProcess(): check if all processes are completed to exit loop
		+ pushQueue(): function to push all events into queue or user request list
			at the beginning or during performing
		+ wait_in_queue(): push back event exceed Quantum time back to queue
		+ core_request(): let core request get CPU core and perform
		+ disk_request(): let disk request get Disk and perform
		+ user_request(): user request will be execute and update User Request Table
		+ update_system_time(): update public varible sysTime
		+ core_request_complete(): check if any core is completed, and process the next event
		+ disk_request_complete(): check if disk is completed, and process next event
		+ user_request_complete(): check if user request complete, and process next event
		+ preset(): function to preset when executing processes at the beginning
************************************************************************************/
//	Function to check if there is any free core
//	then return that free core.
//	Otherwise, return NULL
Core* freeCore()
{
	for (int i = 0; i < numbOfCore; i++)
	{
		if (core[i].checkIdle(sysTime)) return &core[i];
	}
	return NULL;
}
//	Function to check if Disk is free
//	then return. Otherwise, return NULL
Disk *freeDisk()
{
	if (disk->checkIdle(sysTime)) return disk;
	return NULL;
}
//	Function call to return the number of busy core in processing
int numbOfBusyCore(Core *core)
{
	int count = 0;
	for (int i = 0; i < numbOfCore; i++)
	{
		if (core[i].getStatus() == "In Process") count++;
	}
	return count;
}
//	Function call to print out Process Table when there's a completing process
void terminate(List* processList, InputList* inputList, Core* core, Disk* disk)
{
	List* ptr;
	ptr = processTable->getList(processTable);
	cout << "Process " << processList->processID << " terminates at t = " << sysTime << endl;
	cout << "Number of busy core: " << numbOfBusyCore(core) << endl;
	if (disk->checkIdle(sysTime))
		cout << "Disk is IDLE" << endl;
	else
		cout << "Disk is BUSY" << endl;
	cout << "Ready queue: " << readyQueue.size()  << endl;
	if (readyQueue.size() != 0)
		readyQueue.print(&readyQueue);
	cout << "Disk queue: " << diskQueue.size() << endl;
	if (diskQueue.size() != 0)
		diskQueue.print(&diskQueue);
	cout << "Process \t" << "Start time \t" << "Status" << endl;
	do
	{
		cout << ptr->processID << "\t\t" << ptr->startTime << "\t\t" << ptr->status << endl;
		if (ptr->next != NULL) ptr = ptr->next;
		else break;
	} while (1);
	cout << "\n\n";
}
//	Function to return the minimum end time of each processing CPU Core
int minCoreExeTime()
{
	int min = INT_MAX;
	for (int i = 0; i < numbOfCore; i++)
	{
		if (core[i].getStatus() == "In Process")
		{
			if (core[i].getExeTime() <= min) min = core[i].getExeTime();
		}
	}
	return min;
}
//	Function to check if Process Table is completed to exit executing loop
bool doneProcess()
{
	if (processTable->complete(processTable))
		return true;
	return false;
}
//	Push all arriving events into queue, user request table
void pushQueue(List* processList)
{
	List* ptr;
	ptr = processTable->getList(processTable);
	do
	{
		if (ptr->status == "NOT RUNNING" && ptr->startTime <= sysTime)
		{
			if (ptr->currentProcess->getOperation() == "CORE")
			{
				readyQueue.push(ptr);
				processTable = processTable->update(processTable, 0, ptr->processID, ptr->currentProcess, "READY");
			}
			else
			{
				diskQueue.push(ptr);
				processTable = processTable->update(processTable, 0, ptr->processID, ptr->currentProcess, "READY");
			}
		}
		if (ptr->next != NULL) ptr = ptr->next;
		else break;
	} while (1);
	if (processList != NULL)
	{
		if (processList->currentProcess->getOperation() == "CORE")
		{
			if (readyQueue.empty())
				readyQueue.push(processList);
			else if (!readyQueue.empty() && !readyQueue.exist(processList))
				readyQueue.push(processList);
		}
		else if (processList->currentProcess->getOperation() == "DISK")
		{
			if (diskQueue.empty())
				diskQueue.push(processList);
			else if (!diskQueue.empty() && !diskQueue.exist(processList))
				diskQueue.push(processList);
		}
		else if (!userRequest->exist(userRequest, processList))
			userRequest->push(userRequest, processList->startTime, 0, processList->processID, processList->currentProcess, "READY");
	}
}
//	Push back core request to queue when it exceed Quantum time
void wait_in_queue(InputList* list, List* processList)
{
	pushQueue(processList);
	processTable = processTable->update(processTable, 0, list->getProcessID(), list, "READY");
}
//	Function to perform core request 
//	Set CPU core busy, update event info in the Input Table
//	Push back to queue if process exceed Quantum time
void core_request(InputList *input, List *process, Core *core, bool preset)
{
	if (input != NULL && input->getOperation() == "CORE" && input->getMeter() != 0)
	{
		if (freeCore() != NULL)
		{
			readyQueue.pop();
			core = freeCore();
			core->setStatus("In Process");
			core->setCurrentProcess(input);
			if (input->getMeter() <= slice)
			{
				if (preset)
					core->setExeTime(process->startTime + input->getMeter());
				else
					core->setExeTime(sysTime + input->getMeter());
				input = input->updateData(input, 0, true);
			}
			else
			{
				if (preset)
					core->setExeTime(process->startTime + slice);
				else
					core->setExeTime(sysTime + slice);
				input = input->updateData(input, input->getMeter() - slice, false);
			}
		}
		else
			wait_in_queue(input, process);
	}
}
//	Function to perform disk request
//	Set Disk is busy
void disk_request(InputList* input, List* process, Disk* disk)
{
	if (input != NULL && input->getOperation() == "DISK" && input->getMeter() != 0)
	{
		if (freeDisk() != NULL)
		{
			diskQueue.pop();
			disk = freeDisk();
			disk->setStatus("In Process");
			disk->setExeTime(sysTime + input->getMeter());
			disk->setCurrentProcess(input);
			input = input->updateData(input, 0, true);
		}
		else
		{
			wait_in_queue(input, process);
		}
	}
}
//	Function to perform user request (DISPLAY/INPUT)
//	update event info in Input Table
//	update execution time of event in User Request Table to keep track when it's done
void user_request(InputList* input, List* process)
{
	if (!userRequest->complete(userRequest))
	{
		process = userRequest->getList(userRequest);
		do
		{
			if (process->endTime == 0)
				userRequest = userRequest->update(userRequest, sysTime + process->currentProcess->getMeter(), process->currentProcess->getProcessID(), process->currentProcess, "BLOCKED");
			input = process->currentProcess;
			input = input->updateData(input, 0, true);
			if (process->next != NULL) process = process->next;
			else break;
		} while (1);
	}
}
//	Function call to update sysTime
//	There are multiple events in processing, and each of them has different end time.
//	So, need to find minimum end time to update sysTime so that it will not mess up time frame
void update_system_time()
{
	if (disk->getStatus() == "Free" && userRequest->complete(userRequest) && numbOfBusyCore(core) != 0)
		sysTime = minCoreExeTime();
	else if (disk->getStatus() == "In Process" && userRequest->complete(userRequest) && numbOfBusyCore(core) == 0)
		sysTime = disk->getExeTime();
	else if (disk->getStatus() == "Free" && !userRequest->complete(userRequest) && numbOfBusyCore(core) == 0)
		sysTime = userRequest->minEndTime(userRequest);
	else if (disk->getStatus() == "Free" && !userRequest->complete(userRequest) && numbOfBusyCore(core) != 0)
	{
		if (minCoreExeTime() <= userRequest->minEndTime(userRequest))
			sysTime = minCoreExeTime();
		else
			sysTime = userRequest->minEndTime(userRequest);
	}
	else if (disk->getStatus() == "In Process" && userRequest->complete(userRequest) && numbOfBusyCore(core) != 0)
	{
		if (minCoreExeTime() <= disk->getExeTime())
			sysTime = minCoreExeTime();
		else
			sysTime = disk->getExeTime();
	}
	else if (disk->getStatus() == "In Process" && !userRequest->complete(userRequest) && numbOfBusyCore(core) == 0)
	{
		if (disk->getExeTime() <= userRequest->minEndTime(userRequest))
			sysTime = disk->getExeTime();
		else
			sysTime = userRequest->minEndTime(userRequest);
	}
	else if (disk->getStatus() == "In Process" && !userRequest->complete(userRequest) && numbOfBusyCore(core) != 0)
	{
		if (minCoreExeTime() <= disk->getExeTime() && minCoreExeTime() <= userRequest->minEndTime(userRequest))
			sysTime = minCoreExeTime();
		else if (disk->getExeTime() <= minCoreExeTime() && disk->getExeTime() <= userRequest->minEndTime(userRequest))
			sysTime = disk->getExeTime();
		else if (userRequest->minEndTime(userRequest) <= disk->getExeTime() && userRequest->minEndTime(userRequest) <= minCoreExeTime())
			sysTime = userRequest->minEndTime(userRequest);
	}
}
//	Function call to check if there's any user request complete
//	then pop out User Request Table
//	move to the next event and update Process Table
void user_request_complete(InputList* input, List* process)
{
	if (!userRequest->complete(userRequest) && userRequest->minEndTime(userRequest) <= sysTime)
	{
		process = userRequest->getList(userRequest);
		do
		{
			if (process->endTime <= sysTime)
			{
				input = process->currentProcess;
				userRequest = userRequest->update(userRequest, sysTime, input->getProcessID(), input, "TERMINATED");
				input = input->returnActiveProcess(input);
				if (input != NULL)
				{
					List* temp;
					processTable = processTable->update(processTable, 0, input->getProcessID(), input, "READY");
					temp = processTable->searchList(processTable, input->getProcessID());
					pushQueue(temp);
				}
				else
				{
					input = process->currentProcess;
					processTable = processTable->update(processTable, sysTime, input->getProcessID(), input, "TERMINATED");
					terminate(process, input, core, disk);
					processTable = processTable->pop(processTable);
				}
			}
			if (process->next != NULL) process = process->next;
			else break;
		} while (1);
		userRequest = userRequest->pop(userRequest);
	}
}

bool check_event_exist(InputList * input)
{
	for (int i = 0; i < numbOfCore; i++)
	{
		if (core[i].getStatus() == "In Process" && core[i].getCurrentProcess() == input)
			return true;
	}
	return false;
}
//	Function to check if there's any core request complete
//	Mark CPU core is free
//	move to the next event and update Process Table
void core_request_complete(InputList *input, List *process, Core *core)
{
	if (numbOfBusyCore(core) != 0)
	{
		for (int i = 0; i < numbOfCore; i++)
		{
			if (core[i].getStatus() == "In Process" && core[i].getExeTime() <= sysTime)
			{
				core[i].setStatus("Free");
				input = core[i].getCurrentProcess();
				if (!check_event_exist(input) && input->getMeter() == 0)
				{
					input = input->returnActiveProcess(input);
					if (input != NULL && input->getOperation() == "DISK" && input->getMeter() == 0)
					{
						input = input->updateData(input, 0, true);
						input = input->returnActiveProcess(input);
					}
					if (input != NULL)
					{
						core[i].setCurrentProcess(NULL);
						if (input->getOperation() == "DISPLAY" || input->getOperation() == "INPUT")
							processTable = processTable->update(processTable, 0, input->getProcessID(), input, "BLOCKED");
						else
							processTable = processTable->update(processTable, 0, input->getProcessID(), input, "READY");
						process = processTable->searchList(processTable, input->getProcessID());
						pushQueue(process);
					}
					else
					{
						input = core[i].getCurrentProcess();
						process = processTable->searchList(processTable, input->getProcessID());
						processTable = processTable->update(processTable, sysTime, input->getProcessID(), input, "TERMINATED");
						core[i].setStatus("Free");
						core[i].setCurrentProcess(NULL);
						terminate(process, input, core, disk);
						processTable = processTable->pop(processTable);
					}
				}
				else if (input != NULL && input->getMeter() != 0)
				{
					core[i].setCurrentProcess(NULL);
					process = processTable->searchList(processTable, input->getProcessID());
					wait_in_queue(input, process);
				}
			}
		}
	}
}
//	Function to check if Disk is completed
//	Set Disk is free
//	Move to the next event and update Process Table
void disk_request_complete(InputList* input, List* process, Disk* disk)
{
	if (disk->getStatus() == "In Process" && disk->getExeTime() <= sysTime)
	{
		input = disk->getCurrentProcess();
		input = input->returnActiveProcess(input);
		if (input != NULL)
		{
			disk->setCurrentProcess(NULL);
			processTable = processTable->update(processTable, 0, input->getProcessID(), input, "READY");
			process = processTable->searchList(processTable, input->getProcessID());
			pushQueue(process);
		}
		else
		{
			input = disk->getCurrentProcess();
			processTable = processTable->update(processTable, sysTime, input->getProcessID(), input, "TERMINATED");
			terminate(process, input, core, disk);
			processTable = processTable->pop(processTable);
		}
	}
}
//	Function to perform preset all events at the beginning to avoid messing up time frame
//	At the beginning, we might start at least one process
//	While process(es) is/are performing, there might be another arrival process
//	We only have one public sysTime to keep track the current time of system
//	If we wait until the first process(es) is/are completed, and update sysTime
//	then performing arrival process, there might be a problem with time frame
//	So, preset function will solve the problem and set time frame correctly at the beginning
void preset()
{
	List* process , *ptr;
	InputList* input;
	Core* tempCore = core;
	int count = 2;
	process = ptr = NULL;
	input = NULL;
	pushQueue(NULL);
	do
	{
		if (!readyQueue.empty())
		{
			int temp = 0;
			if (readyQueue.size() < (numbOfCore - numbOfBusyCore(core)))
				temp = readyQueue.size();
			else
				temp = numbOfCore - numbOfBusyCore(core);
			while (temp != 0)
			{
				process = readyQueue.front();
				input = process->currentProcess;
				processTable = processTable->update(processTable, 0, input->getProcessID(), input, "RUNNING");
				core_request(input, process, tempCore, true);
				temp--;
			}
		}
		ptr = processTable->getList(processTable);
		do
		{
			if (ptr->status == "NOT RUNNING" && ptr->startTime <= minCoreExeTime())
			{
				pushQueue(ptr);
				processTable = processTable->update(processTable, 0, ptr->processID, ptr->currentProcess, "READY");
			}
			if (ptr->next != NULL) ptr = ptr->next;
			else break;
		} while (1);
	} while (--count != 0 && numbOfBusyCore(core) != 0);
}
//	Function to create loop to execute all processes in Process Table until it's done
void Controller::execute()
{
//	Create variables to be used
	List *processList1, *processList2, *processList3;
	InputList *inputList1, *inputList2, *inputList3;
	Core *tempCore;
	Disk *tempDisk;
//	Initialize variables
	processList1 = processList2 = processList3 = NULL;
	inputList1 = inputList2 = inputList3 = NULL;
	tempDisk = disk; tempCore = core;
//	Calling function preset() to initialize Queues and process timing at the beginning
//	At the beginning, we might start at least one process
//	While process(es) is/are performing, there might be another arrival process
//	We only have one public sysTime to keep track the current time of system
//	If we wait until the first process(es) is/are completed, and update sysTime
//	then performing arrival process, there might be a problem with time frame
//	So, preset function will solve the problem and set time frame correctly at the beginning
	preset();	
//	Create a loop to perform all processes until all are done
	do
	{
	//	Checking if there's any event in queue 
	//	We might have single or multiple core
	//	Base on the number of free core, and the number of events are in queue
	//	Pop out and let the process get core
	//	Update status of each process in Process Table
		if (!readyQueue.empty())
		{
			int temp = 0;
			if (readyQueue.size() < (numbOfCore - numbOfBusyCore(core)))
				temp = readyQueue.size();
			else
				temp = numbOfCore - numbOfBusyCore(core);
			while (temp != 0)
			{
				processList1 = readyQueue.front();
				inputList1 = processList1->currentProcess;
				processTable = processTable->update(processTable, 0, inputList1->getProcessID(), inputList1, "RUNNING");
				core_request(inputList1, processList1, tempCore, false);
				temp--;
			}
		}
	//	We have only one disk, just check if any disk request, then pop out queue to process.
		if (!diskQueue.empty())
		{
			processList2 = diskQueue.front();
			inputList2 = processList2->currentProcess;
			processTable = processTable->update(processTable, 0, inputList2->getProcessID(), inputList2, "RUNNING");
		}

		disk_request(inputList2, processList2, tempDisk);
		user_request(inputList3, processList3);		// calling function to process User Request (DISPLAY/INPUT)
		update_system_time();		// calling function to update system time
//		if (!userRequest->complete(userRequest) && minCoreExeTime() == userRequest->minEndTime(userRequest))
//			core_request_complete(inputList1, processList1, tempCore);
	//	Calling functions to check if there is any request completion
		user_request_complete(inputList3, processList3);
		core_request_complete(inputList1, processList1, tempCore);
		disk_request_complete(inputList2, processList2, tempDisk);
		freeDisk();
		
	} while (!doneProcess());
}
/***********************************************************************************/

/***********************************************************************************/
/*	- Clean up all link lists after completing all processes					   */
/***********************************************************************************/
void Controller::clean()
{
	inputTable->clean(inputTable);
	delete inputTable;
}
/***********************************************************************************/
