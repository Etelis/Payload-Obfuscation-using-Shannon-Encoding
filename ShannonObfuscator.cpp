//
// Created by Eteli on 18/12/2022.
//

#include "ShannonObfuscator.h"

std::vector<unsigned char> ShannonObfuscator::divideIntoChunks(
        const std::vector<unsigned char> &payload, const std::vector<unsigned char> &lowEntropyPattern) {
    // Check that the low-entropy pattern is not empty
    if (lowEntropyPattern.empty()) {
        throw std::invalid_argument("Low-entropy pattern cannot be empty");
    }

    // Check that the payload is not empty
    if (payload.empty()) {
        throw std::invalid_argument("Payload cannot be empty");
    }

    // Calculate the chunk size
    size_t chunkSize = lowEntropyPattern.size() / 8;
    if (chunkSize == 0) {
        throw std::invalid_argument("Low-entropy pattern must be at least 8 bits long");
    }

    // Divide the payload into chunks and insert the low-entropy pattern between each chunk
    std::vector<unsigned char> dividedPayload;
    for (size_t i = 0; i < payload.size(); i += chunkSize) {
        dividedPayload.insert(dividedPayload.end(), payload.begin() + i,
                              payload.begin() + std::min(i + chunkSize, payload.size()));
        dividedPayload.insert(dividedPayload.end(), lowEntropyPattern.begin(), lowEntropyPattern.end());
    }

    return dividedPayload;
}

std::vector<unsigned char> ShannonObfuscator::insertLowEntropyPattern(
        const std::vector<unsigned char> &payload, const std::vector<unsigned char> &lowEntropyPattern) {
    // Check that the low-entropy pattern is not empty
    if (lowEntropyPattern.empty()) {
        throw std::invalid_argument("Low-entropy pattern cannot be empty");
    }

    // Check that the payload is not empty
    if (payload.empty()) {
        throw std::invalid_argument("Payload cannot be empty");
    }

    // Calculate the chunk size
    size_t chunkSize = lowEntropyPattern.size() / 8;
    if (chunkSize == 0) {
        throw std::invalid_argument("Low-entropy pattern must be at least 8 bits long");
    }

    // Divide the payload into chunks and insert the low-entropy pattern between each chunk
    std::vector<unsigned char> dividedPayload;
    for (size_t i = 0; i < payload.size(); i += chunkSize) {
        dividedPayload.insert(dividedPayload.end(), payload.begin() + i,
                              payload.begin() + std::min(i + chunkSize, payload.size()));
        dividedPayload.insert(dividedPayload.end(), lowEntropyPattern.begin(), lowEntropyPattern.end());
    }

    return dividedPayload;
}


std::vector<unsigned char> ShannonObfuscator::createShannonEncoding(const std::vector<unsigned char> &payload) {
        // A map to hold the frequencies of each symbol
        std::map<unsigned char, double> symbolFrequencies;

        // Calculate the frequencies of each symbol
        for (unsigned char c: payload) {
            symbolFrequencies[c]++;
        }
        for (std::pair<const unsigned char, double> p: symbolFrequencies) {
            p.second /= payload.size();
        }

        // Create the encodings for each symbol using the frequencies
        std::vector<unsigned char> prefix;
        createEncoding(symbolFrequencies, prefix);

        // Create the low-entropy pattern by concatenating the encodings for each symbol
        std::vector<unsigned char> lowEntropyPattern;
        for (unsigned char c: payload) {
            // Find the encoding for the current symbol
            auto it = this->_symbolEncodings.find(c);
            if (it == this->_symbolEncodings.end()) {
                throw std::invalid_argument("Symbol not found in encodings");
            }
            // Concatenate the encoding to the low-entropy pattern
            lowEntropyPattern.insert(lowEntropyPattern.end(), it->second.begin(), it->second.end());
        }

        return lowEntropyPattern;
    }


void ShannonObfuscator::createEncoding(const std::map<unsigned char, double> &frequencies,
                                       std::vector<unsigned char> prefix) {
    if (frequencies.size() == 1) {
        // If there is only one symbol, assign the prefix as its encoding
        std::pair<unsigned char, double> p = *frequencies.begin();
        this->_symbolEncodings[p.first] = prefix;
        return;
    }

    // Split the symbols into two sets with approximately equal frequencies
    std::map<unsigned char, double> left, right;
    double leftSum = 0, rightSum = 0;
    for (std::pair<unsigned char, double> p : frequencies) {
        (leftSum <= rightSum ? left : right)[p.first] = p.second;
        (leftSum <= rightSum ? leftSum : rightSum) += p.second;
    }

    // Create encodings for the left set with a 0 appended to the prefix
    std::vector<unsigned char> leftPrefix = prefix;
    leftPrefix.push_back(0);
    createEncoding(left, leftPrefix);

    // Create encodings for the right set with a 1 appended to the prefix
    std::vector<unsigned char> rightPrefix = prefix;
    rightPrefix.push_back(1);
    createEncoding(right, rightPrefix);
}

std::vector<unsigned char> ShannonObfuscator::restoreOriginalPayload(
        const std::vector<unsigned char> &data, const std::vector<unsigned char> &lowEntropyPattern) {
        // Check that the low-entropy pattern is not empty
        if (lowEntropyPattern.empty()) {
            throw std::invalid_argument("Low-entropy pattern cannot be empty");
        }

        // Check that the data is not empty
        if (data.empty()) {
            throw std::invalid_argument("Data cannot be empty");
        }

        // Calculate the chunk size
        size_t chunkSize = lowEntropyPattern.size() / 8;
        if (chunkSize == 0) {
            throw std::invalid_argument("Low-entropy pattern must be at least 8 bits long");
        }

        // Divide the data into chunks
        std::vector<std::vector<unsigned char>> chunks;
        size_t i = 0;

        while (i < data.size()) {
            if (i + chunkSize < data.size()) {
                // If the current chunk is not the last chunk, it consists of chunkSize bytes
                chunks.push_back(std::vector<unsigned char>(data.begin() + i, data.begin() + i + chunkSize));
                i += chunkSize + lowEntropyPattern.size();
            } else {
                break;
            }
        }


        // Restore the original payload by concatenating the chunks
        std::vector<unsigned char> originalPayload;
        for (const auto &chunk: chunks) {
            originalPayload.insert(originalPayload.end(), chunk.begin(), chunk.end());
        }

        while (size(originalPayload) > this->payload_original_size){
            originalPayload.pop_back();
        }

        return originalPayload;
}


std::vector<unsigned char> ShannonObfuscator::Obfuscate(const std::vector<unsigned char> &payload) {
    this->payload_original_size = std::size(payload);
    this->_lowEntropyPattern = this->createShannonEncoding(payload);
    return insertLowEntropyPattern(payload, this->_lowEntropyPattern);
}

std::vector<unsigned char> ShannonObfuscator::Reverse(const std::vector<unsigned char> &payload) {
    return restoreOriginalPayload(payload, this->_lowEntropyPattern);
}