#include <iostream>
#include <string>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include "ParserClass.hpp"
#include "parsedStruct.hpp"
#include "PyCode.hpp"

std::string GetCurDirByStr()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    return static_cast<std::string>(buff);
}

int generatePyCode(const ParsedStruct parsedData)
{
    PyCode PyObj(parsedData);
    std::string dir = GetCurDirByStr() + "/bin";
    if (PyObj.mainFunc(dir) == 0)
    {
        return 0;
    }
    return -1;
}

int main(int argc, char* argv[])
{
    ParserClass ParserDB;
    while(true)
    {
        std::cout << "Pick Reading Method, Type \"FILE\" or \"STREAM\". Type QUIT/Q/q to quit: " << std::endl;
        std::string MethodStr;
        std::cin >> MethodStr;
        if (MethodStr == "STREAM")
        {
            ParserDB.readInput();
            ParserDB.parseSelectAttr();
            ParserDB.parseHavingConds();
            ParserDB.parseMFStruct();
            ParsedStruct parsedData = ParserDB.getParsed();
            generatePyCode(parsedData);
            /*
             * Generate Python Code here
             */
        }
        else if (MethodStr == "FILE")
        {
            /*
            * Read From File and Generate Python Code
            */
        }
        else if (MethodStr == "QUIT" || MethodStr == "q" || MethodStr == "Q")
        {
            std::cout << "Bye!" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "Wrong Input. Please try again!" << std::endl;
            continue;
        }
    }
    return 0;
}