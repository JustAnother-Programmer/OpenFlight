/*
* Copyright (c) 2022 - The OpenFlight Team
*
* Logger.h
*/

#pragma once

enum logLevel {
	LOG_LVL_ERR = 0,
	LOG_LVL_WRN = 1,
	LOG_LVL_INFO = 2,
	LOG_LVL_DEBUG = 3,
};

class Logger
{
public:
	bool initializeLogging();
	void cleanup();
	void logOut(logLevel lvl, const char* msg);

private:
	bool logToFile;
	bool logToConsole;
};