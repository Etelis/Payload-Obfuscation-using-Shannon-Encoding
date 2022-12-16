#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<std::vector<unsigned char>> divideIntoChunks(const std::vector<unsigned char>& payload, const std::vector<unsigned char>& lowEntropyPattern) {
    // The vector of chunks
    std::vector<std::vector<unsigned char>> chunks;

    // The size of the low-entropy pattern
    size_t patternSize = lowEntropyPattern.size();

    // The current chunk
    std::vector<unsigned char> currentChunk;

    // Iterate through the payload
    for (auto c : payload) {
        // If the current chunk is larger than the low-entropy pattern, start a new chunk
        if (currentChunk.size() > patternSize) {
            chunks.push_back(currentChunk);
            currentChunk.clear();
        }

        // Add the current character to the current chunk
        currentChunk.push_back(c);
    }

    // Add the final chunk
    if (!currentChunk.empty()) {
        chunks.push_back(currentChunk);
    }

    return chunks;
}

std::vector<unsigned char> insertLowEntropyPattern(const std::vector<std::vector<unsigned char>>& chunks, const std::vector<unsigned char>& lowEntropyPattern) {
    // The vector of bytes that will hold the final data
    std::vector<unsigned char> data;

    // Iterate through the chunks
    for (size_t i = 0; i < chunks.size(); i++) {
        // Add the current chunk
        for (auto c : chunks[i]) {
            data.push_back(c);
        }

        // If this is not the last chunk, add the low-entropy pattern
        if (i < chunks.size() - 1) {
            for (auto c : lowEntropyPattern) {
                data.push_back(c);
            }
        }
    }

    return data;
}