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

std::string GetCurDirByStr()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    return static_cast<std::string>(buff);
}

int main(int argc, char* argv[])
{
    std::cout << "Pick Reading Method, Type \"FILE\" or \"STREAM\". Type QUIT/Q/q to quit: " << std::endl;
    std::string MethodStr;
    std::cin >> MethodStr;
    ParserClass ParserDB;
    while(true)
    {
        if (MethodStr == "STREAM")
        {
            ParserDB.readInput();
            ParserDB.parseSelectAttr();
            ParserDB.parseHavingConds();
            ParserDB.parseMFStruct();
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
        else if (MethodStr == "QUIT" || MethodStr == "" || MethodStr == "Q")
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