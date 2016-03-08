#include <iostream>
#include <math.h>
#include "binary.h"
#include <random>
#include <chrono>
#include <thread>

#define MAX_OF_PQ 100

void printBinary(const binary& x)
{
    for(int i=x.size()-1; i>=0; --i)
    {
        std::cout << x.getNumberAt(i);
    }
    std::cout << std::endl;
}

binary initBinaryResult(int n) // initializes an n long number
{
    std::vector<bool> init(n);
    binary result(init);

    return result;
}

bool powerOf2 (long long int x) // decides if the number is power of 2 or not
{
    while (((x % 2) == 0) && x > 1)
    {
        x /= 2;
    }

    return (x == 1);
}

binary convertDecToBin(long long int x) // decimal -> binary converter
{
    binary result;

    if(x == 1)
    {
        result = initBinaryResult(1);
        result.setNumberAt(0,1);
        return result;
    }

    if(powerOf2(x))
    {
        int index = log2(x);
        result = initBinaryResult(index + 1);
        result.setNumberAt(index, 1);
        return result;
    }

    int sup = 1;
    int n = 0;
    while(sup < x)
    {
        sup = sup << 1;
        n++;
    }

    result = initBinaryResult(n);

    sup /= 2;
    n--;

    while(x != 0)
    {
        if(sup <= x)
        {
            x -= sup;
            result.setNumberAt(n, true);
        }
        else
        {
            result.setNumberAt(n, false);
        }
        sup /= 2;
        n--;
    }

    return result;
}

long long int convertBinToDec(const binary& x) // binary -> decimal converter
{
    long long int result = 0;

    for(int i=x.size()-1; i>=0; --i)
    {
        if(x.getNumberAt(i))
        {
            result += pow(2,i);
        }
    }

    return result;
}

binary operator+(const binary& left, const binary& right)
{
    int n = left.size();
    binary result = initBinaryResult(n);

    binary rightFilled = right;
    if(right.size() < n)
    {
        rightFilled.fill(n);
    }

    binary leftFilled = left;
    if(left.size() < n)
    {
        leftFilled.fill(n);
    }

    bool overflow = false;
    for(int i=0; i<n; ++i)
    {
        bool leftI = leftFilled.getNumberAt(i);
        bool rightI = rightFilled.getNumberAt(i);

        result.setNumberAt(i, leftI ^ rightI ^ overflow);

        if((leftI & rightI)
           || (overflow & (leftI | rightI))
          )
        {
            overflow = true;
        }
        else
        {
            overflow = false;
        }
    }
    if (overflow == true)
    {
        result.resize(n+1);
        result.setNumberAt(n, true);
    }

    return result;
}

binary operator+(const binary& left, int x)
{
    binary right = convertDecToBin(x);

    return left + right;
}

binary operator-(const binary& left, const binary& right)
{
    int n = left.size();
    binary result = initBinaryResult(n);

    binary rightTemp = right; // 2-complement
    rightTemp.fill(n);

    for(int i=0; i<n; ++i)
    {
        rightTemp.setNumberAt(i, !rightTemp.getNumberAt(i));
    }
    rightTemp = rightTemp + 1;

    bool overflow = false;
    for(int i=0; i<n; ++i)
    {
        bool leftI = left.getNumberAt(i);
        bool rightI = rightTemp.getNumberAt(i);

        result.setNumberAt(i, leftI ^ rightI ^ overflow);

        if((leftI & rightI)
           || (overflow & (leftI | rightI))
          )
        {
            overflow = true;
        }
        else
        {
            overflow = false;
        }
    }

    return result;
}

binary operator*(const binary& left, const binary& right)
{
    binary result = initBinaryResult(left.size());
    int howMuch = left.size();

    if(right.powerOf2())
    {
        long long int temp = convertBinToDec(left);
        temp = temp << (howMuch - 1);
        return convertDecToBin(temp);
    }

    if(left.powerOf2())
    {
        long long int temp = convertBinToDec(right);
        temp = temp << (howMuch - 1);
        return convertDecToBin(temp);
    }

    binary tempR = right;
    binary tempL = left;
    if(left.size() > right.size())
    {
        tempR.fill(left.size());
    }
    else
    {
        tempL.fill(right.size());
    }

// Egyptian method
    for(int i=0; i<howMuch; ++i)
    {
        if(tempL.getNumberAt(i))
        {
            result = tempR + result;
        }
        tempR = tempR + tempR;
    }

    return result;
}

binary power(const binary& x, int n)
{
    binary y = convertDecToBin(1);
    if(n == 0) return y;

// exponentiation by squaring
    binary temp = x;
    while(n>1)
    {
        if(n%2 == 0)
        {
            n /= 2;
        }
        else
        {
            y = y * temp;
            n = (n-1) / 2;
        }
        temp = temp * temp;
    }

    return y * temp;
}

bool generateSecret() // generates a random bit
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 secret(seed); // Mersenne twister
    return secret()%2;
}

bool isPrime(int x, int accuracy) // Miller-Rabin test
{
    if(x < 3 || x%2 == 0) return false;

    int temp = x-1;
    int d = 0;
    while(temp % 2 == 0)
    {

    }

}

std::pair<int, int> generatePrimes() // generates two 2-digit distinct primes
{
    int p, q;
    do
    {
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 prime1(seed1);
        p = prime1() % (MAX_OF_PQ + 1);
    } while(!isPrime(p, 10));

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 prime2(seed2);
    q = prime2() % (MAX_OF_PQ + 1);

    while(p == q || !isPrime(q, 10));
    {
        q += 2;
    }

    if(p > q)
    {
        int temp = p;
        p = q;
        q = temp;
    }

    std::pair<int, int> result;
    result.first = p;
    result.second = q;
std::cout << "\n\n\n" << p << " " << q << "\n\n\n";
    return result;
}

int gcd(long long int a, long long int b) // greatest common divisor
{
    // Eucledian algorithm
    while(b != 0)
    {
        long long int t = b;
        b = a % b;
        a = t;
    }

    return a;
}

binary generateE(const binary& x) // finds a coprime to x (result < x)
{
    binary result;
    long long int coprime;
    long long int y = convertBinToDec(x);

    do
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 secret(seed);
        coprime = secret()%1000;
    } while(coprime > y || gcd(coprime, y) != 1);

    result = convertDecToBin(coprime);

    return result;
}

binary generateRandom(int size, const binary& n = binary()) // generates random binary number
{
    int nInt;
    if(n.size() == 0)
    {
        nInt = 1;
    }
    else
    {
        nInt = convertBinToDec(n);
    }

    binary result = initBinaryResult(size);
    int resultInt;

    do
    {
        for(int i=0; i<size; ++i)
        {
            result.setNumberAt(i, generateSecret());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        resultInt = convertBinToDec(result);
    } while(resultInt == 0 || gcd(resultInt, nInt) != 1);

    return result;
}

binary chooseRs(const binary& a, const binary& b) // chooses which secret we want
{
    return a;
}

binary computeD(const binary& e, const binary& fiN) // extended Euclidean alg.
{
    long long int xn, xn1, xn2;
    long long int yn, yn1, yn2;
    long long int rn, rn1, rn2;
    long long int n = 0;

    xn = 1;
    xn1 = 0;
    yn = 0;
    yn1 = 1;
    rn = convertBinToDec(fiN);
    rn1 = convertBinToDec(e);

    while(rn1 != 0)
    {
        int q = rn / rn1;
        rn2 = rn % rn1;
        xn2 = xn - xn1*q;
        yn2 = yn - yn1*q;
        n++;

        rn = rn1;
        rn1 = rn2;
        xn = xn1;
        xn1 = xn2;
        yn = yn1;
        yn1 = yn2;
    }

    if(yn < 0)
    {
        yn += convertBinToDec(fiN);
    }

    return convertDecToBin(yn);
}

binary RSA(const binary& base, const binary& exponent, const binary& mod)
{
    long long int result = 1;
    long long int baseInt = convertBinToDec(base);
    long long int exponentInt = convertBinToDec(exponent);
    long long int modInt = convertBinToDec(mod);

// modular exponentation
    baseInt = baseInt % modInt;
    while(exponentInt > 0)
    {
        if(exponentInt % 2 == 1)
        {
            result = (result * baseInt) % modInt;
        }
        exponentInt = exponentInt >> 1;
        baseInt = (baseInt * baseInt) % modInt;
    }

    return convertDecToBin(result);
}

binary lsb(const binary& x) // least significant bit
{
    binary result = initBinaryResult(1);
    result.setNumberAt(0,x.getNumberAt(0));
    return result;
}

void correctBit(binary& bin)
{
    if(bin.size() == 2)
    {
        bin.resize(1);
    }
}

bool computeWantedSecret(const binary& m, const binary& rs)
{
    return !(lsb(rs).getNumberAt(0) == lsb(m).getNumberAt(0));
}
