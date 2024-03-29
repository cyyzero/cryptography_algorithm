#ifndef __SHA_H
#define __SHA_H

#include <cstdint>
#include <string>
#include <vector>
#include <array>

class SHA512
{
public:
    const static uint64_t sha512_k[];
    SHA512(const std::string& raw);
    std::string get_hash() const;
private:
    void padding_and_append();
    void calculate();

    std::array<uint64_t, 8> hash_;
    std::vector<uint8_t> msg_;
};

#endif