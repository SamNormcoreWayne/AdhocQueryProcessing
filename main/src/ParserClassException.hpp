#include <string>
#include <exception>

namespace ParserClassException
{
    struct ZeroSizeException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "The size of the container is 0!";
        }
    };

    struct FileNotOpenException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Cannot open this file. Please Check";
        }
    };
}

