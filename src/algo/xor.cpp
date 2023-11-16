/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** xor.cpp
*/

#include <iostream>
#include <string>
#include <getopt.h>
#include <sstream>
#include <iomanip>
#include "../include/struct.h"

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