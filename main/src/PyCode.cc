#include <iostream>
#include <exception>
#include <fstream>
#include "PyCode.hpp"
#include "ParserClassException.hpp"

int PyCode::main(std::string cwd) noexcept
{
    try
    {
        this->generateMFStructPy(cwd);
        this->generatePostgresConPy(cwd);
        this->generateMainPy(cwd);
    }
    catch(ParserClassException::FileNotOpenException &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

static std::string generateTab(int i)
{
    std::string outStr;
    for (int j = 0; j < i; ++j)
    {
        outStr += "\t";
    }
    return outStr;
}

int PyCode::generateMFStructPy(std::string cwd) const throw()
{
    std::ofstream ofs;
    ofs.open(cwd + "\\MFStruct.py", std::ios::out);
    if (!ofs.is_open())
    {
        ofs.clear();
        ofs.close();
        throw ParserClassException::FileNotOpenException();
    }
    else
    {
        ofs << "from collections import defaultdict";
        ofs << std::endl;
        ofs << "class MFStruct():" << std::endl;
        ofs << PyCode::generateTab(1) + "def __main__(self, var : list, aggFuncs : defaultdict(int)):" << std::endl;
        ofs << PyCode::generateTab(2) + "\'\'\'" << std::endl;
        ofs << PyCode::generateTab(3) + "var : list<str> - select attributes names" << std::endl;
        ofs << PyCode::generateTab(3) + "aggFuncs<aggFunc, value> : dict<str, int> - aggFuncs in select" << std::endl;
        ofs << PyCode::generateTab(2) + "self.var = var\n" + PyCode::generateTab(2) + "self.aggFunc = aggFuncs\n" << std::endl;
        ofs.clear();
        ofs.close();
    }
}