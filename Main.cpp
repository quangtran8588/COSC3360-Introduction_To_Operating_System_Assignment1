/***********************************************************************
	Quang Tran - PSID: 1290921
	COSC 3360 - Fundamental to Operating System
	Assignment 1
************************************************************************/
#include "Controller.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
// Create variables and initialize 
	string data, temp = "";
	Controller control = Controller();
	ifstream infile;
//	infile.open("input.txt");
//	while (!infile.eof())
// Create while loop to read all data in text file and store in Input Table
	while (getline(cin, data))
	{
//		getline(infile, data);
		if (data == "")	{}
		else if (data.at(0) == 'N' && data.at(1) == 'C' && data.at(2) == 'O' && data.at(3) == 'R' && data.at(4) == 'E')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
		else if (data.at(0) == 'S' && data.at(1) == 'L' && data.at(2) == 'I' && data.at(3) == 'C' && data.at(4) == 'E')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
		else if (data.at(0) == 'N' && data.at(1) == 'E' && data.at(2) == 'W')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
		else if (data.at(0) == 'C' && data.at(1) == 'O' && data.at(2) == 'R' && data.at(3) == 'E')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
		else if (data.at(0) == 'D' && data.at(1) == 'I' && data.at(2) == 'S' && data.at(3) == 'K')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
		else if (data.at(0) == 'D' && data.at(1) == 'I' && data.at(2) == 'S' && data.at(3) == 'P' && data.at(4) == 'L')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
		else if (data.at(0) == 'I' && data.at(1) == 'N' && data.at(2) == 'P' && data.at(3) == 'U' && data.at(4) == 'T')
		{
			stringstream ss;
			ss << data;
			getline(ss, data, ' ');
			getline(ss, temp);
			control.setInputTable(data, temp);
		}
	}
//	Create Process Table then execute all processes
//	Finally, cleaning up all link lists after completing all processes
	control.createProcessTable();
	control.execute();
	control.clean();
//	system("pause");
	return 0;
}