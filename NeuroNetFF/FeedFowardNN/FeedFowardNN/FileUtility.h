#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include <stdio.h>

using namespace std;

class FileUtility
{
public:
	void OpenFile(char *fileName, char mode);
	char * GetNextRow();
	int WriteLine(char *text);
	bool IsEndOfFile();
	void CloseFile();
	~FileUtility()
	{
		if (_File != NULL)
		{
			CloseFile();
		}
	}
private:
	FILE *_File;
	int nbytes = 1024;
	char *_Line = (char *)malloc(nbytes);
	char *_Buffer = (char *)malloc(nbytes);
	size_t _Bytes_read;
	int _Linesize = 0;
};

void FileUtility::OpenFile(char* fileName, char mode)
{
	errno_t errorNumber;
	
	switch (mode)
	{
	case 'r':
		errorNumber = fopen_s(&_File,fileName, "r");
		break;
	case 'w':
		errorNumber = fopen_s(&_File, fileName, "a");
		break;
	default:
		errorNumber = fopen_s(&_File, fileName, "r");
		break;
	}

	if (_File == NULL && errorNumber != 0)
		printf("Failed to open the file");
}
bool FileUtility::IsEndOfFile()
{
	return _File == NULL ? true : feof(_File) == 0 ? true : false ;
}

char * FileUtility::GetNextRow()
{
	if (_File == NULL)
	{
		printf("Failed to open the file to read.");
		return NULL;
	}
	
	if (fgets(_Buffer, sizeof(_Buffer), _File) != NULL)
	{
		_Buffer[strlen(_Buffer) - 1] = '\0';
		return _Buffer;
	}

	printf("Failed to read the file.");

	return  NULL;
}
int FileUtility::WriteLine(char *text)
{
	if (_File == NULL)
	{
		printf("Failed to open the file to write.");
		return 1;
	}
	fputs(text, _File);
	return 0;
}
void  FileUtility::CloseFile()
{
	free(_Buffer);
	free(_Line);
}