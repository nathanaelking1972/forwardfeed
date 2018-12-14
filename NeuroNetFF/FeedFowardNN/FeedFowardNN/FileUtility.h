#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class FileUtility
{
public:
	void OpenFile(char *fileName, char mode);
	string GetNextRow();
	int WriteLine(char *text);
	bool IsEndOfFile();
	void CloseFile();
	~FileUtility()
	{
		
	}
private:
	ifstream _FileRead;
	ofstream _FileWrite;
};

void FileUtility::OpenFile(char* fileName, char mode)
{
	
	if (mode == 'r')
	{
  		 _FileRead.open(fileName);
	}
	else if (mode = 'w')
	{
		 _FileWrite.open(fileName);
	}
	else
	{
		throw exception("invalid file operation");
	}

	
}
bool FileUtility::IsEndOfFile()
{
	return _FileRead.eof();
}

string FileUtility::GetNextRow()
{
	string line;
	if (_FileRead.eof())
	{
		printf("Failed to open the file to read.");
		return NULL;
	}
	
	if (!_FileRead.eof() && getline(_FileRead, line, '\n'))
	{
	
		return line;
	}
	else
	{
		return "";
	}

}
int FileUtility::WriteLine(char *text)
{
	
	_FileWrite << text << endl;
	return 0;
}
void  FileUtility::CloseFile()
{
	_FileRead.close();
	_FileWrite.close();
	
}