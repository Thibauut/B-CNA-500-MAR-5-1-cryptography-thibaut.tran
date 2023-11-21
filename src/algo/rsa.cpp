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
#include <gmpxx.h>

std::string mpzClassToLittleEndianHex(const mpz_class& number) {
    size_t size = mpz_sizeinbase(number.get_mpz_t(), 256);
    unsigned char* buffer = new unsigned char[size];
    mpz_export(buffer, nullptr, -1, 1, -1, 0, number.get_mpz_t());
    std::string result;
    for (size_t i = 0; i < size; ++i) {
        char hexByte[3];
        sprintf(hexByte, "%02x", buffer[i]);
        result += hexByte;
    }
    delete[] buffer;
    return result;
}

mpz_class convertBigEndianToLittleEndian(const mpz_class& bigEndianNumber) {
    size_t sizeInBytes = mpz_sizeinbase(bigEndianNumber.get_mpz_t(), 256);
    unsigned char* littleEndianBuffer = new unsigned char[sizeInBytes];
    mpz_export(littleEndianBuffer, nullptr, -1, 1, 0, 0, bigEndianNumber.get_mpz_t());
    std::reverse(littleEndianBuffer, littleEndianBuffer + sizeInBytes);
    mpz_class littleEndianNumber;
    mpz_import(littleEndianNumber.get_mpz_t(), sizeInBytes, -1, 1, 0, 0, littleEndianBuffer);
    delete[] littleEndianBuffer;
    return littleEndianNumber;
}

mpz_class convertLittleEndianToBigEndian(const mpz_class& littleEndianNumber) {
    size_t sizeInBytes = mpz_sizeinbase(littleEndianNumber.get_mpz_t(), 256);
    unsigned char* bigEndianBuffer = new unsigned char[sizeInBytes];
    mpz_export(bigEndianBuffer, nullptr, -1, 1, 0, 0, littleEndianNumber.get_mpz_t());
    std::reverse(bigEndianBuffer, bigEndianBuffer + sizeInBytes);
    mpz_class bigEndianNumber;
    mpz_import(bigEndianNumber.get_mpz_t(), sizeInBytes, -1, 1, 0, 0, bigEndianBuffer);
    delete[] bigEndianBuffer;
    return bigEndianNumber;
}


std::string reverseTwo(std::string& input) {
    std::string reversed;
    for (size_t i = 0; i < input.length(); i += 2)
        reversed = input.substr(i, 2) + reversed;
    return reversed;
}


unsigned int hex_pair_to_int(const std::string& hex_pair) {
    std::stringstream ss(hex_pair);
    unsigned int value;
    ss >> std::hex >> value;
    return value;
}

mpz_class rsa_encrypt(const mpz_class& plaintext, const mpz_class& public_key_n, const mpz_class& public_key_e) {
    mpz_class ciphertext;
    mpz_powm(ciphertext.get_mpz_t(), plaintext.get_mpz_t(), public_key_e.get_mpz_t(), public_key_n.get_mpz_t());
    return ciphertext;
}

mpz_class rsa_decrypt(mpz_class& ciphertext, mpz_class& public_key_n, mpz_class& private_key_d) {
    mpz_class plaintext;
    mpz_powm(plaintext.get_mpz_t(), ciphertext.get_mpz_t(), private_key_d.get_mpz_t(), public_key_n.get_mpz_t());
    return plaintext;
}

std::string rsaKeyGen(Arguments args) {
    std::string result;

    mpz_class value1;
    mpz_set_str(value1.get_mpz_t(), args.rsa_arg1.c_str(), 16);
    mpz_class value2;
    mpz_set_str(value2.get_mpz_t(), args.rsa_arg2.c_str(), 16);

    mpz_class p = value1;
    mpz_class q = value2;
    mpz_class n = p * q;
    mpz_class d;
    mpz_class totient = (p - 1) * (q - 1);

    mpz_class e;
    if (mpz_sizeinbase(value1.get_mpz_t(), 2) == 8 || mpz_sizeinbase(value2.get_mpz_t(), 2) == 8)
        e = 257;
    if (mpz_sizeinbase(value1.get_mpz_t(), 2) == 256 || mpz_sizeinbase(value2.get_mpz_t(), 2) == 256)
        e = 65537;
    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), totient.get_mpz_t());

    std::string e_str = "0" + e.get_str(16);
    std::string n_str = n.get_str(16);
    std::string d_str = d.get_str(16);

    n = convertBigEndianToLittleEndian(n);
    d = convertBigEndianToLittleEndian(d);

    std::cout << "public key: " << std::hex << "0" << e << "-" << n << std::endl;
    std::cout << "private key: " << std::hex << d << "-" << n << std::endl;

    return result;
}

std::string insertSpaceBetweenChars(std::string& input, size_t spaceInterval) {
    std::string result;
    for (size_t i = 0; i < input.length(); i += spaceInterval) {
        result += input.substr(i, spaceInterval);
        if (i + spaceInterval < input.length())
            result += " ";
    }
    return result;
}

std::vector<std::string> parseHexString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, delimiter))
        result.push_back(token);
    return result;
}

std::string calcRsa(Arguments args, std::string message) {

    std::string result;

    //parse message
    size_t spaceInterval = 0;
    std::vector<std::string> messageHex;
    if (args.generate == true) {
        message = insertSpaceBetweenChars(message, 2);
        messageHex = parseHexString(message, ' ');
    }
    if (args.generate == false) {
        messageHex = parseHexString(message, 'u');
        if (messageHex.size() == 1)
            messageHex.at(0) = reverseTwo(messageHex.at(0));
    }

    //parse key
    size_t hyphenPos = args.key.find('-');

    std::string e = args.key.substr(0, hyphenPos);
    std::string n = args.key.substr(hyphenPos + 1);
    mpz_class value_e;
    mpz_set_str(value_e.get_mpz_t(), e.c_str(), 16);
    value_e = convertLittleEndianToBigEndian(value_e);
    mpz_class value_n;
    mpz_set_str(value_n.get_mpz_t(), n.c_str(), 16);
    value_n = convertLittleEndianToBigEndian(value_n);

    // algo
    if (args.generate == true) {
        for (auto &a: messageHex) {
            mpz_class plaintext;
            mpz_set_str(plaintext.get_mpz_t(), a.c_str(), 16);
            mpz_class ciphertext = rsa_encrypt(plaintext, value_n, value_e);

            std::ostringstream oss;
            oss << std::hex << ciphertext;
            std::string hexString = oss.str();
            result += hexString + "u";
        }
        result.pop_back();

        mpz_class plaintext;
        mpz_set_str(plaintext.get_mpz_t(), result.c_str(), 16);
        if (result.length() == 4)
            result = mpzClassToLittleEndianHex(plaintext);
    }
    if (args.generate == false) {
        for (auto &a: messageHex) {
            mpz_class ciphertext;
            mpz_set_str(ciphertext.get_mpz_t(), a.c_str(), 16);
            mpz_class decrypted_text = rsa_decrypt(ciphertext, value_n, value_e);

            std::ostringstream oss;
            oss << std::hex << decrypted_text;
            std::string hexString = oss.str();
            if (hexString.size() == 1)
                hexString = "0" + hexString;
            result += hexString;
        }
    }
    return result;
}