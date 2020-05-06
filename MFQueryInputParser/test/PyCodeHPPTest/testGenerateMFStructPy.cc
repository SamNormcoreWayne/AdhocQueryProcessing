#include <iostream>
#include <string>
#include "../../src/PyCode.hpp"

int main()
{
    PyCode obj;
    std::string path = "/home/samwayne/document/MFQuery/main/bin";
    obj.generateMFStructPy(path);
    return 0;
}