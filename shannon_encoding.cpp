#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>

// A map to hold the symbol encodings
std::map<unsigned char, std::vector<unsigned char>> symbolEncodings;

// A function to create a Shannon encoding for a given payload
std::vector<unsigned char> createShannonEncoding(const std::vector<unsigned char>& payload) {
    // A map to hold the frequencies of each symbol
    std::map<unsigned char, double> symbolFrequencies;

    // Calculate the frequencies of each symbol
    for (unsigned char c: payload) {
        symbolFrequencies[c]++;
    }
    for (std::pair<const unsigned char, double> p: symbolFrequencies) {
        p.second /= payload.size();
    }

    // Create the low-entropy pattern by concatenating the encodings for each symbol

    std::vector<unsigned char> lowEntropyPattern;
    for (unsigned char c: payload) {
        // Find the encoding for the current symbol
        auto it = symbolEncodings.find(c);
        if (it == symbolEncodings.end()) {
            throw std::invalid_argument("Symbol not found in encodings");
        }
        // Concatenate the encoding to the low-entropy pattern
        lowEntropyPattern.insert(lowEntropyPattern.end(), it->second.begin(), it->second.end());
    }

    return lowEntropyPattern;
}



// A helper function to create a Shannon encoding for a given set of symbols and frequencies
void createEncoding(const std::map<unsigned char, double>& frequencies, std::vector<unsigned char> prefix) {
    if (frequencies.size() == 1) {
        // If there is only one symbol, assign the prefix as its encoding
        std::pair<unsigned char, double> p = *frequencies.begin();
        symbolEncodings[p.first] = prefix;
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
