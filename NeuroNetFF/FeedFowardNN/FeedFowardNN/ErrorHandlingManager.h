#pragma once
#include "FileUtility.h"

class ErrorHandlingManager
{
public:
	void LogError(char *Error);
private:
	char _fileName[];
};

void ErrorHandlingManager::LogError(char *Error)
{
	FileUtility fileUtility;
	fileUtility.OpenFile(".\\ErrorLog.txt", 'a');
	fileUtility.WriteLine(Error);
	fileUtility.CloseFile();
}
