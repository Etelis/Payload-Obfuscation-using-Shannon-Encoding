//
// Created by Eteli on 18/12/2022.
//
#include "Obfuscator.h"
#include "map"

#ifndef SHANNONOBFUSCATOR_SHANNONOBFUSCATOR_H
#define SHANNONOBFUSCATOR_SHANNONOBFUSCATOR_H


class ShannonObfuscator : public Obfuscator{
public:
    std::vector<unsigned char> Obfuscate(const std::vector<unsigned char> &payload) override;
    std::vector<unsigned char> Reverse(const std::vector<unsigned char> &payload) override;
private:
    std::map<unsigned char, std::vector<unsigned char>> _symbolEncodings;
    std::vector<unsigned char> _lowEntropyPattern;
    size_t payload_original_size;
    std::vector<unsigned char> divideIntoChunks(const std::vector<unsigned char> &payload, const std::vector<unsigned char>& lowEntropyPattern);
    std::vector<unsigned char> insertLowEntropyPattern(const std::vector<unsigned char> &payload, const std::vector<unsigned char>& lowEntropyPattern);
    std::vector<unsigned char> restoreOriginalPayload(const std::vector<unsigned char>& data, const std::vector<unsigned char>& lowEntropyPattern);
    std::vector<unsigned char> createShannonEncoding(const std::vector<unsigned char>& payload);
    void createEncoding(const std::map<unsigned char, double>& frequencies, std::vector<unsigned char> prefix);
};


#endif //SHANNONOBFUSCATOR_SHANNONOBFUSCATOR_H
