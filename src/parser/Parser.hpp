/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** Parser.hpp
*/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>

class ParserArg {
    public:
        ParserArg(int argc, char **argv) {
            _argc = argc;
            for (int i = 0; i < argc; i++)
                _args.push_back(argv[i]);
        };
        ~ParserArg() {};

        bool isArg(std::string arg, std::string option="-") {
            std::string finalArg = option + arg;
            for (std::string line : _args)
                if (line.find(finalArg) != std::string::npos) {
                    return true;
                }
            return false;
        };

        std::vector<std::string> getArg(std::string arg, std::string option="-") {
            std::vector<std::string> result;
            bool canStock = false;
            std::string finalArg = option + arg;
            for (std::string line : _args) {
                if (canStock)
                    result.push_back(line);
                if (line.find(finalArg) != std::string::npos)
                    canStock = true;
            }
            return result;
        };

        std::string getNextArg(std::string arg, std::string option="-") {
            std::string finalArg = option + arg;
            for (int i = 0; i < _args.size(); i++) {
                if (_args.at(i).find(finalArg) != std::string::npos && i < _args.size() - 1)
                    return _args.at(i + 1);
            }
            return "";
        };

        std::string getKeyArg(std::string arg, std::string option="-") {
            std::string finalArg = option + arg;
            for (int i = 0; i < _args.size(); i++) {
                if (_args.at(i).find(finalArg) != std::string::npos && i < _args.size() - 1) {
                    if (_args.at(i + 1).find("-") != std::string::npos) {
                        std::string key = _args.at(i + 2);
                        return key;
                    } else {
                        std::string key = _args.at(i + 1);
                        return key;
                    }
                }
            }
            return "";
        };

        void getRsaArgs(std::string &arg1, std::string &arg2) {
            std::string finalArg = "-g";
            for (size_t i = 0; i < _args.size(); ++i) {
                if (_args[i] == "-g" && i + 2 < _args.size()) {
                    arg1 = _args[i + 1];
                    arg2 = _args[i + 2];
                    return;
                }
            }
            std::cerr << "Error: Arguments after -g not found or incomplete." << std::endl;
            exit(84);
        };

    private:
        int _argc;
        std::vector<std::string> _args;
};