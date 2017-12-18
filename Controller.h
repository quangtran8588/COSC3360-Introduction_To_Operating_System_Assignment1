#pragma once
#include <string>
#include <iostream>
#include "InputTable.h"
#include "Core.h"
#include "Disk.h"
#include "ProcessTable.h"
#include "InputList.h"
#include "Queue.h"
#include <limits.h>

using namespace std;

class Controller
{	
public:
	Controller();
	~Controller();
	void setInputTable(string data1, string data2);
	void createProcessTable();
	void execute();
	void clean();
};

