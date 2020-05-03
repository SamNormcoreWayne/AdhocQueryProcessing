#pragma once
#include <string>
#include <vector>
#include "parsedStruct.hpp"

/*
 * This class defines some functions to generate CODE that can operates
 * the DB system to show the output of our MFQuery.
 * The expected output is under bin/, which are: main.py and postgresCon.py
 */

class PyCode
{
private:
    ParsedStruct mfStruct;
protected:
    static std::string generateTab(int i)
    {
        std::string outStr;
        for (int j = 0; j < i; ++j)
        {
            outStr += "\t";
        }
        return outStr;
    }

    int generateMFStructPy(std::string) throw();
    int generatePostgresConPy(std::string) throw();
    int generateMainPy(std::string) throw();
public:
    PyCode() = default;
    PyCode(const ParsedStruct& oldStruct)
    {
        this->mfStruct = oldStruct;
    }
    PyCode& operator= (const ParsedStruct& oldStruct)
    {
        this->mfStruct = oldStruct;
        return *this;
    }

    int mainFunc(std::string) noexcept;
};