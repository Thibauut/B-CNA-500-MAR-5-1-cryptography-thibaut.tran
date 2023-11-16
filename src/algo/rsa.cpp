/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** rsa.cpp
*/

#include <iostream>
#include <string>
#include <getopt.h>
#include <sstream>
#include <iomanip>
#include "../include/struct.h"

long long hexStringToNum(const std::string& hexString) {
    std::istringstream converter(hexString);
    long long value;
    converter >> std::hex >> value;
    return value;
}

long long mod_pow(long long base, long long exponent, long long mod) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exponent /= 2;
    }
    return result;
}

long long mod_inverse(long long a, long long m) {
    long long m0 = m, t, q, x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

std::string reverseHexBytes(const std::string& hexStr) {
    std::string reversedStr;
    for (size_t i = hexStr.length(); i > 0; i -= 2) {
        reversedStr += hexStr.substr(i - 2, 2);
    }
    return reversedStr;
}

std::string rsaKeyGen(Arguments args) {
    std::string result;
    long long p = hexStringToNum(args.rsa_arg1);
    long long q = hexStringToNum(args.rsa_arg2);


    std::cout << "p: " << p << std::endl;
    std::cout << "q: " << q << std::endl;

    long long n = p * q;
    long long lambda = (p - 1) * (q - 1);
    long long e = 65537;
    long long d = mod_inverse(e, lambda);

    std::stringstream ss;
    ss << "0" << std::hex << e;
    std::string e_str = ss.str();
    ss.str("");

    ss << std::hex << n;
    std::string n_str = ss.str();
    // n_str = reverseHexBytes(n_str);
    ss.str("");

    ss << std::hex << d;
    std::string d_str = ss.str();
    // d_str = reverseHexBytes(d_str);
    ss.str("");

    std::cout << "public key: " << std::hex << e_str << "-" << n_str << std::endl;
    std::cout << "private key: " << std::hex << d_str << "-" << n_str << std::endl;


    return result;
}

std::string calcRsa(Arguments args, std::string message) {
    std::string result;

    return result;
}