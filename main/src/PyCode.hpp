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
    int generateMFStructPy(std::string) const throw();
    int generatePostgresConPy(std::string) const throw();
    int generateMainPy(std::string) const throw();

    static std::string generateTab(int);
public:
    PyCode() = default;
    PyCode(const ParsedStruct& oldStruct)
    {
        this->mfStruct = oldStruct;
    }
    PyCode& operator= (const ParsedStruct& oldStruct)
    {
        this->mfStruct = oldStruct;
    }

    int main(std::string);
};