#include <iostream>
#include "ShannonObfuscator.cpp"

int main() {
    // The original payload
    std::vector<unsigned char> payload = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    auto OB = ShannonObfuscator();

    auto OB_payload = OB.Obfuscate(payload);

    auto back = OB.Reverse(OB_payload);
//    // Create the Shannon-based low-entropy pattern
//    std::vector<unsigned char> lowEntropyPattern = createShannonEncoding(payload);
//
//    // Divide the payload into chunks
//    std::vector<std::vector<unsigned char>> chunks = divideIntoChunks(payload, lowEntropyPattern);
//
//    // Insert the low-entropy pattern between each chunk
//    std::vector<unsigned char> data = insertLowEntropyPattern(chunks, lowEntropyPattern);
//
//    // Restore the original payload
//    std::vector<unsigned char> restoredPayload = restoreOriginalPayload(data, lowEntropyPattern);


    // Print the original and restored payloads
    std::cout << "Original payload: ";
    for (auto c: payload) {
        std::cout << std::hex << c << ' ';
    }
    std::cout << '\n';

    std::cout << "Obfuscated payload: ";
    for (auto c: OB_payload) {
        std::cout << std::hex << c << ' ';
    }

    std::cout << '\n';

    std::cout << "Restored payload: ";
    for (auto c: back) {
        std::cout << std::hex << c << ' ';
    }
}