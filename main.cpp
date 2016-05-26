#include <iostream>
#include <thread>
#include "binary.h"

#define SIZEOF_MESSAGE 8
#define SECRETS true
#define DEBUG true

void printBinary(const binary&);
bool generateSecret(); // generates a random bit
binary generateRandom(int, const binary& = binary());
std::pair<int, int> generatePrimes();
binary generateE(const binary&);
binary convertDecToBin(long long int); // decimal -> binary converter
long long int convertBinToDec(const binary&); // binary -> decimal converter
binary computeD(const binary&, const binary&); // extended Euclidean alg.
binary RSA(const binary&, const binary&, const binary&);
bool lsb(const binary&); // least significant bit
bool computeWantedSecret(const binary&, bool);
void correctBit(binary&);

int main()
{
// initialize secrets
    const bool b0 = generateSecret();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    const bool b1 = generateSecret();
    if(DEBUG || SECRETS)
    {
        std::cout << "Secrets: " << b0 << " " << b1 << std::endl << std::endl;
    }
	
// choose
	const bool choice = generateSecret();
	
    if(DEBUG || SECRETS)
    {
        std::cout << "choice: " << choice << std::endl;
        std::cout << std::endl;
    }

// generating primes
    std::pair<int, int> primes = generatePrimes();
    binary p = convertDecToBin(primes.first);
    binary q = convertDecToBin(primes.second);
    if(DEBUG)
    {
        std::cout << "Primes: " << convertBinToDec(p) << " " << convertBinToDec(q) << std::endl;
        printBinary(p);
        printBinary(q);
        std::cout << std::endl;
    }

// calculating n, fi(n)
    binary n = p * q;
    if(DEBUG)
    {
        std::cout << "n = p*q: " << convertBinToDec(n) << std::endl;
        printBinary(n);
        std::cout << std::endl;
    }

    binary fiN = n - p - q + 1;
    if(DEBUG)
    {
        std::cout << "fiN: " << convertBinToDec(fiN) << std::endl;
        printBinary(fiN);
        std::cout << std::endl;
    }

// generate message
    binary m = generateRandom(SIZEOF_MESSAGE, n);
    // 0 < m < n && gcd(m,n) == 1
    if(DEBUG)
    {
        std::cout << "Message: " << convertBinToDec(m) << std::endl;
        printBinary(m);
        std::cout << std::endl;
    }

// generating e (public key)
    binary e = generateE(fiN);
    if(DEBUG)
    {
        std::cout << "e: " << convertBinToDec(e) << std::endl;
        printBinary(e);
        std::cout << std::endl;
    }

// compute d (private key)
    binary d = computeD(e, fiN);
    if(DEBUG)
    {
        std::cout << "d: " << convertBinToDec(d) << std::endl;
        printBinary(d);
        std::cout << std::endl;
    }

// compute c_s, c_{1-s} (cr)
    binary real = RSA(m, e, n);
	binary madeUp = generateRandom(real.size());
	
	// if sender sends real data or random bits
	bool sendReal = generateSecret();
	if(!sendReal)
	{
		real = generateRandom(madeUp.size());
	}
	
	if(DEBUG)
	{
		std::cout << "send real: " << sendReal << std::endl << std::endl;
	}
	
	binary cs, cr;
	
	if(!choice)
	{
		cs = real;
		cr = madeUp;
	}
	else
	{
		cs = madeUp;
		cr = real;
	}
	
    if(DEBUG)
    {
        std::cout << "cs: " << convertBinToDec(cs) << std::endl;
        printBinary(cs);
        std::cout << std::endl;
		
		std::cout << "cr: " << convertBinToDec(cr) << std::endl;
        printBinary(cr);
        std::cout << std::endl;
    }

// compute m0, m1
    binary ms = RSA(cs, d, n);
    if(DEBUG)
    {
        std::cout << "ms: " << convertBinToDec(ms) << std::endl;
        printBinary(ms);
        std::cout << std::endl;
    }

    binary mr = RSA(cr, d, n);
    if(DEBUG)
    {
        std::cout << "mr: " << convertBinToDec(mr) << std::endl;
        printBinary(mr);
        std::cout << std::endl;
    }
// compute r0, r1
    bool r0 = lsb(ms);
    if(DEBUG)
    {
        std::cout << "r0: " << r0 << std::endl;
        std::cout << std::endl;
    }

    bool r1 = lsb(mr);
    if(DEBUG)
    {
        std::cout << "r1: " << r1 << std::endl;
        std::cout << std::endl;
    }

// compute r0+b0, r1+b1
    bool r0b0 = r0 ^ b0;
    bool r1b1 = r1 ^ b1;
    if(DEBUG)
    {
        std::cout << "r0b0, r1b1: " << r0b0 << " " << r1b1 << std::endl;
        std::cout << std::endl;
    }

// compute wanted secret
    bool bs;

	if(!choice) bs = computeWantedSecret(m, r0b0);
	else bs = computeWantedSecret(m, r1b1);
    if(DEBUG || SECRETS)
    {
        std::cout << "Wanted secret: " << bs << std::endl;
    }

    return 0;
}
