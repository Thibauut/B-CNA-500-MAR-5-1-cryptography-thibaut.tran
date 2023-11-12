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

    private:
        int _argc;
        std::vector<std::string> _args;
};