//
// Created by Eteli on 18/12/2022.
//
#include <iostream>
#include <vector>

#ifndef SHANNONOBFUSCATOR_OBFUSCATOR_H
#define SHANNONOBFUSCATOR_OBFUSCATOR_H


class Obfuscator {
public:
    // Obfuscate a given payload and return the obfuscated version
    virtual std::vector<unsigned char> Obfuscate(const std::vector<unsigned char>& payload) = 0;

    // Reverse the obfuscation of a given payload and return the original version
    virtual std::vector<unsigned char> Reverse(const std::vector<unsigned char>& payload) = 0;

};


#endif //SHANNONOBFUSCATOR_OBFUSCATOR_H
