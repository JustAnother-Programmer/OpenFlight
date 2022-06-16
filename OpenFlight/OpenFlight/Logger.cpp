/*
* Copyright (c) 2022 - The OpenFlight Team
*
* Logger.cpp
*/

#include <iostream>
#include <Windows.h>

#include "Logger.h"

bool Logger::initializeLogging()
{
	// TODO: Initialize log file etc in here (Open file handle)
	// TODO: From command line parameters setup if stuff should be logged to a file, console or both

	return true;
}

void Logger::cleanup()
{
	// TODO: Clean up here and maybe save any log files or queued entries
	// TODO: Close any file handles as well
}

void Logger::logOut(logLevel lvl, const char* msg)
{
	const char* logLevelMsg[4] = { "[ERROR]: ", "[WARNING]: ", "[INFO]: ", "[DEBUG]: " };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// TODO: Allow for extra paramters to be passed into the function such as numbers etc for better logging

	switch (lvl)
	{
	case LOG_LVL_ERR:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

		std::cout << logLevelMsg[lvl] << msg << std::endl;
		break;
	case LOG_LVL_WRN:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);

		std::cout << logLevelMsg[lvl] << msg << std::endl;
		break;
	case LOG_LVL_INFO:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

		std::cout << logLevelMsg[lvl] << msg << std::endl;
		break;
	case LOG_LVL_DEBUG:
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);

		std::cout << logLevelMsg[lvl] << msg << std::endl;
		break;
	default:
		break;
	}
}
