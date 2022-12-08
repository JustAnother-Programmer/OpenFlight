/*
* Copyright (c) 2022 - The OpenFlight Team
*
* FileManager.cpp
*/

#include <iostream>
#include <fstream>

#include "FileManager.h"
#include "Logger.h"

const char* FileManager::readFile(const char* fileName)
{
    std::ifstream data;
    int num;

    data.open(fileName);
    if (!data)
    {
        
    }

    data >> num;

    while (!data.eof())
    {
        data >> num;
    }

    data.close();

    return nullptr;
}
