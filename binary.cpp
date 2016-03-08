#include "binary.h"

void binary::fill(int n)
{
    number.resize(n);
    for(int i=number.size()+1; i<n; ++i)
    {
        number[i] = false;
    }
}

bool binary::powerOf2() const
{
    int c = 0;

    for(int i=0; i<number.size(); ++i)
    {
        if(number[i] == 1) c++;
    }

    return (c == 1);
}

void binary::setNumberFromCharArray(const char* c)
{
    for(int i=0; i<number.size(); ++i)
    {
        for(int j=7; j>=0; --j)
        {
            number[i] = (c[i] >> j) & 0x1;
        }
    }
}

binary& binary::operator=(const binary& other)
{
    if(this != &other)
    {
        this->setNumber(other.getNumber());
    }
    return *this;
}
