#ifndef BINARY_H_INCLUDED
#define BINARY_H_INCLUDED

#include <vector>

class binary // class to handle binary numbers
{
    std::vector<bool> number; // binary representation

public:

    binary() { }
    binary(std::vector<bool> b): number(b) {}

    binary& operator=(const binary& other);

    // getter-setters
    std::vector<bool> getNumber() const { return number; }
    void setNumber(const std::vector<bool> &newNumber) { number = newNumber;}

    bool getNumberAt(int k) const { return number[k]; }
    void setNumberAt(int k, bool b) { number[k] = b; }

    void fill(int); // fill the number with 0-s
    bool powerOf2() const; // decides if the number is power of 2 or not

    int size() const { return number.size(); }
    void resize(int n) { number.resize(n); }

    friend binary operator+(const binary&, const binary&);
    friend binary operator+(const binary&, int);
    friend binary operator-(const binary&, const binary&);
    friend binary operator*(const binary&, const binary&);
};

#endif // BINARY_H_INCLUDED
