/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** struct.h
*/

#pragma once

#include <string>
#include <iostream>

struct Arguments {
    std::string algorithm;
    std::string mode;
    std::string key;
    std::string message;
    bool blockMode;
    bool generate;
    std::string primeP;
    std::string primeQ;

    void print() {
        std::cout << "algorithm: " << algorithm << std::endl;
        std::cout << "mode: " << mode << std::endl;
        std::cout << "key: " << key << std::endl;
        std::cout << "message: " << message << std::endl;
        std::cout << "blockMode: " << blockMode << std::endl;
        std::cout << "generate: " << generate << std::endl;
        std::cout << "primeP: " << primeP << std::endl;
        std::cout << "primeQ: " << primeQ << std::endl;
    }
} args_t ;