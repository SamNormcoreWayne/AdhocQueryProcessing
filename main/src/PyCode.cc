#include <iostream>
#include <exception>
#include <fstream>
#include "PyCode.hpp"
#include "ParserClassException.hpp"

int PyCode::mainFunc(std::string cwd) noexcept
{
    try
    {
        std::cout << "Now generating MFStruct.py" << std::endl;
        this->generateMFStructPy(cwd);
        std::cout << "Now generating PostrgresCon.py" << std::endl;
        this->generatePostgresConPy(cwd);
        std::cout << "Now generating Main.py" << std::endl;
        this->generateMainPy(cwd);
    }
    catch(ParserClassException::FileNotOpenException &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

int PyCode::generateMFStructPy(std::string cwd) throw()
{
    std::ofstream ofs;
    std::string path = cwd + "/test_MFStruct.py";
    ofs.open(path, std::ios::out);
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
        ofs << generateTab(1) + "def __main__(self, var : list, aggFuncs : defaultdict(int)):" << std::endl;
        ofs << generateTab(2) + "\'\'\'" << std::endl;
        ofs << generateTab(3) + "var : list<str> - select attributes names" << std::endl;
        ofs << generateTab(3) + "aggFuncs<aggFunc, value> : dict<str, int> - aggFuncs in select" << std::endl;
        ofs << generateTab(2) + "\'\'\'" << std::endl;
        ofs << generateTab(2) + "self.var = var\n" + PyCode::generateTab(2) + "self.aggFunc = aggFuncs\n" << std::endl;
        ofs.clear();
        ofs.close();
    }
    return 0;
}

int PyCode::generateMainPy(std::string cwd) throw()
{
    return 0;
}

int PyCode::generatePostgresConPy(std::string cwd) throw()
{
    return 0;
}