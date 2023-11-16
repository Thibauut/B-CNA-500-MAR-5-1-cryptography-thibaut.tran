/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** main.cpp
*/

#include "parser/Parser.hpp"
#include "algo/xor.cpp"
#include "algo/aes.cpp"
#include "algo/rsa.cpp"

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
    args_s.key = parser.getKeyArg("b");

    if (args_s.key == "" && algorithm != "rsa") {
        printHelp();
        exit(84);
    }

    args_s.rsa_generation = parser.isArg("g");
    if (args_s.rsa_generation == true && algorithm == "rsa")
        parser.getRsaArgs(args_s.rsa_arg1, args_s.rsa_arg2);

    args_s.generate = parser.isArg("c");
    return args_s;
}

int main(int ac, char **av) {
    Arguments args = parseArguments(ac, av);
    std::string line;

    if (args.rsa_generation == true && args.algorithm == "rsa") {
        std::cout << rsaKeyGen(args) << std::endl;
        return 0;
    }
    while (std::getline(std::cin, line)) {
        if (args.blockMode && (args.key.size() != line.size())) {
            std::cerr << "Key size must be the same as the message size" << std::endl;
            exit(84);
        }
        if (args.algorithm == "xor")
            std::cout << calcXor(args, line) << std::endl;
        if (args.algorithm == "aes")
            std::cout << calcAes(args, line) << std::endl;
        if (args.algorithm == "rsa")
            std::cout << calcRsa(args, line) << std::endl;
        if (args.algorithm == "pgp")
            std::cout << "PGP" << std::endl;
    }
    return 0;
}