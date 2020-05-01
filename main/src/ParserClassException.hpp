#include <string>
#include <exception>

struct ZeroSizeException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "The size of the container is 0!";
    }
}ZeroSizeException;