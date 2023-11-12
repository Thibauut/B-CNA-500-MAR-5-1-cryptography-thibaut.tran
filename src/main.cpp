/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** main.cpp
*/

#include <iostream>
#include <string>
#include <getopt.h>
#include "parser/Parser.hpp"
#include "include/struct.h"
#include <sstream>
#include <iomanip>

/*
USAGE
      ./mypgp [-xor | -aes | -rsa] [-c | -d] [-b] KEY
      the MESSAGE is read from standard input
DESCRIPTION
      -xor       computation using XOR algorithm
      -aes       computation using AES algorithm
      -rsa       computation using RSA algorithm
      -pgp       computation using both RSA and AES algorithm
      -c         MESSAGE is clear and we want to cipher it
      -d         MESSAGE is ciphered and we want to decipher it
      -b         block mode: for xor and aes, only works on one block
                 MESSAGE and KEY must be of the same size
      -g P Q     for RSA only: generate a public and private key
                 pair from the prime number P and Q
*/

void printHelp() {
    std::cout << "USAGE" << std::endl;
    std::cout << "\t./mygpg [-xor | -aes | -rsa] [-c | -d] [-b] KEY" << std::endl;
    std::cout << "DESCRIPTION" << std::endl;
    std::cout << "\t-xor\t\tcomputation using XOR algorithm" << std::endl;
    std::cout << "\t-aes\t\tcomputation using AES algorithm" << std::endl;
    std::cout << "\t-rsa\t\tcomputation using RSA algorithm" << std::endl;
    std::cout << "\t-pgp\t\tcomputation using both RSA and AES algorithm" << std::endl;
    std::cout << "\t-c\t\tMESSAGE is clear and we want to cipher it" << std::endl;
    std::cout << "\t-d\t\tMESSAGE is ciphered and we want to decipher it" << std::endl;
    std::cout << "\t-b\t\tblock mode: for xor and aes, only works on one block" << std::endl;
    std::cout << "\t-g P Q\t\tfor RSA only: generate a public and private key" << std::endl;
    std::cout << "\t\t\tpair from the prime number P and Q" << std::endl;
}

Arguments parseArguments(int ac, char **av) {
    std::string algorithm;
    std::vector<std::string> args;

    ParserArg parser(ac, av);
    if (parser.isArg("h")) {
        printHelp();
        exit(0);
    }
    if (parser.isArg("xor")) {
        algorithm = "xor";
        args = parser.getArg("xor");
    } else if (parser.isArg("aes")) {
        algorithm = "aes";
        args = parser.getArg("aes");
    } else if (parser.isArg("rsa")) {
        algorithm = "rsa";
        args = parser.getArg("rsa");
    } else {
        printHelp();
        exit(84);
    }
    Arguments args_s;
    args_s.blockMode = parser.isArg("b");
    args_s.algorithm = algorithm;
    args_s.key = parser.getNextArg("b");
    args_s.generate = parser.isArg("c");
    return args_s;
}

std::string hexToBin(std::string hex) {
    std::string result;
    for (int i = 0; i < hex.size(); i += 2) {
        std::string byte = hex.substr(i, 2);
        char chr = (char) (int) strtol(byte.c_str(), NULL, 16);
        result += chr;
    }
    return result;
}

std::string binToHex(std::string bin) {
    std::string result;
    for (int i = 0; i < bin.size(); i++) {
        char chr = bin[i];
        std::string byte = std::to_string((int) chr);
        result += byte;
    }
    return result;
}

std::string intToHex(unsigned char intValue) {
    std::stringstream stream;
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(intValue);
    return stream.str();
}

std::string stringToHex(std::string str) {
    std::string result;
    for (int i = 0; i < str.size(); i++) {
        char chr = str[i];
        std::string byte = std::to_string((int) chr);
        result += byte;
    }
    return result;
}

std::string calcXor(Arguments args, std::string message) {
    std::string result;
    message = hexToBin(message);
    args.key = hexToBin(args.key);
    for (int i = 0; i < message.size(); i++) {
        char xorResult = message[i] ^ args.key[i];
        result += intToHex(static_cast<unsigned char>(xorResult));
    }
    return result;
}

int main(int ac, char **av) {
    Arguments args = parseArguments(ac, av);
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << calcXor(args, line) << std::endl;
    }
    return 0;
}