/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** pgp.cpp
*/

#include <iostream>
#include <string>
#include <getopt.h>
#include <sstream>
#include <iomanip>
#include "../include/struct.h"
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <random>


std::string generateSymmetricKey() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    for (int i = 0; i < 32; ++i) {
        int randomNibble = dis(gen);
        ss << std::hex << randomNibble;
    }

    return ss.str();
}

void createFileWithResult(const std::string& result) {
    std::ofstream outputFile("ciphered");
    if (outputFile.is_open()) {
        outputFile << result;
        outputFile.close();
    } else {
        std::cerr << "Error: Unable to create the file 'final.txt'" << std::endl;
    }
}

std::string calcPgp(Arguments args, std::string message) {
    std::string result;

    //parse key
    size_t hyphenPos = args.key.find('-');

    std::string e = args.key.substr(0, hyphenPos);
    std::string n = args.key.substr(hyphenPos + 1);
    mpz_class value_e;
    mpz_set_str(value_e.get_mpz_t(), e.c_str(), 16);
    if (args.generate == false && message.size() == 4 && args.key.size() <= 9)
        value_e = convertLittleEndianToBigEndian(value_e);

    mpz_class value_n;
    mpz_set_str(value_n.get_mpz_t(), n.c_str(), 16);
    if (args.generate == false && message.size() == 4 && args.key.size() <= 9)
        value_n = convertLittleEndianToBigEndian(value_n);

    // algo
    if (args.generate == true) {
        unsigned char messageBytes[16];
        unsigned char keyBytes[16];
        unsigned char ciphered[16];

        //generate symmetric key
        std::string cipheredKey;
        std::string symmetricKey = generateSymmetricKey();
        // std::string symmetricKey = "6b50fd39f06d33cfefe6936430b6c94f";
        std::string symmetricKeyInitial = symmetricKey;

        //encrypt the symmetric key with the public key
        mpz_class plaintext;
        mpz_set_str(plaintext.get_mpz_t(), symmetricKey.c_str(), 16);
        mpz_class ciphertext = rsa_encrypt(plaintext, value_n, value_e);

        std::ostringstream oss;
        oss << std::hex << ciphertext;
        cipheredKey = oss.str();

        //encrypt the message with the symmetric key
        for (int i = 0; i < 16; ++i) {
            sscanf(message.c_str() + 2 * i, "%2hhx", &messageBytes[i]);
            sscanf(symmetricKeyInitial.c_str() + 2 * i, "%2hhx", &keyBytes[i]);
        }
        aesEncrypt(messageBytes, keyBytes, ciphered);
        std::ostringstream oss2;
        for (int i = 0; i < 16; ++i)
            oss2 << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ciphered[i]);
        std::string encryptedMessage = oss2.str();

        //create the file with the result
        std::string finalresult = cipheredKey + " " + encryptedMessage;
        createFileWithResult(finalresult);
        result = encryptedMessage;
    }

    if (args.generate == false) {
        unsigned char messageBytes[16];
        unsigned char keyBytes[16];
        unsigned char ciphered[16];

        //decrypt the message with the symmetric key
        size_t spacePos = message.find(' ');

        std::string cipheredKeyToDecrypt = message.substr(0, spacePos);
        std::string cipheredMessageToDecrypt = message.substr(spacePos + 1);
        std::string decryptedKeySymetric;

        mpz_class ciphertext;
        mpz_set_str(ciphertext.get_mpz_t(), cipheredKeyToDecrypt.c_str(), 16);
        mpz_class decrypted_text = rsa_decrypt(ciphertext, value_n, value_e);

        std::ostringstream oss;
        oss << std::hex << decrypted_text;
        decryptedKeySymetric = oss.str();

        //decrypt the message with the symmetric key
        for (int i = 0; i < 16; ++i) {
            sscanf(cipheredMessageToDecrypt.c_str() + 2 * i, "%2hhx", &messageBytes[i]);
            sscanf(decryptedKeySymetric.c_str() + 2 * i, "%2hhx", &keyBytes[i]);
        }

        aesDecrypt(messageBytes, keyBytes, ciphered);

        std::ostringstream oss2;
        for (int i = 0; i < 16; ++i)
            oss2 << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ciphered[i]);
        result = oss2.str();
    }



    return result;
}