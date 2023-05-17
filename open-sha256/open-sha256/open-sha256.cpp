#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <openssl/sha.h>
#include <openssl/evp.h>

#pragma warning(disable : 4996)

//compile with g++ -o osh osh.cpp -lssl -lcrypto
std::string to_hex(const unsigned char* data, size_t len) {
    std::string hex_str;
    static const char hex_chars[] = "0123456789abcdef";

    for (size_t i = 0; i < len; ++i) {
        unsigned char c = data[i];
        hex_str += hex_chars[c / 16];
        hex_str += hex_chars[c % 16];
    }

    return hex_str;
}

std::vector<std::string> generate_guesses(int length) {
    std::vector<std::string> guesses;
    for (int i = 0; i < (1 << (7 * length)); ++i) {
        std::string guess;
        for (int j = 0; j < length; ++j) {
            guess += static_cast<char>((i / (1 << (7 * j))) % 128);
        }
        guesses.push_back(guess);
    }
    return guesses;
}

std::string crack_sha256(const std::string& hash_to_crack, int max_length = 6) {
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    std::string hash;
    unsigned char digest[SHA256_DIGEST_LENGTH];

    for (int i = 1; i <= max_length; ++i) {
        for (const std::string& guess : generate_guesses(i)) {
            SHA256_CTX ctx_copy = ctx;
            SHA256_Update(&ctx_copy, reinterpret_cast<const unsigned char*>(guess.data()), guess.size());
            SHA256_Final(digest, &ctx_copy);

            if (hash_to_crack == to_hex(digest, SHA256_DIGEST_LENGTH)) {
                return guess;
            }
        }
    }

    return ""; // Return an empty string if no match is found
}

int main() {
    std::string hash_to_crack;
    std::cout << "SHA-256-HASH: " << std::endl;//no checks implemented, prob in future there will be
    std::cin >> hash_to_crack;
    std::string cracked = crack_sha256(hash_to_crack);

    if (!cracked.empty()) {
        std::cout << "Hash cracked! Original input was: " << cracked << std::endl;
        std::system("pause");
    }
    else {
        std::cout << "Failed to crack hash." << std::endl;
        std::system("pause");
    }

    return 0;
}
