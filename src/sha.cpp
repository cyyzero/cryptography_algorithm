#include "sha.h"

#include <iostream>
#include <cassert>

#define SHA2_SHFR(x, n) (x >> n)
#define SHA2_ROTR(x, n) ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n) ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z) ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA512_F1(x) (SHA2_ROTR(x, 28) ^ SHA2_ROTR(x, 34) ^ SHA2_ROTR(x, 39))
#define SHA512_F2(x) (SHA2_ROTR(x, 14) ^ SHA2_ROTR(x, 18) ^ SHA2_ROTR(x, 41))
#define SHA512_F3(x) (SHA2_ROTR(x, 1) ^ SHA2_ROTR(x, 8) ^ SHA2_SHFR(x, 7))
#define SHA512_F4(x) (SHA2_ROTR(x, 19) ^ SHA2_ROTR(x, 61) ^ SHA2_SHFR(x, 6))

namespace
{

void sha_unpack64(uint8_t* str, uint64_t x)
{
    for (int i = 0; i < 8; ++i)
    {
        str[i] = static_cast<uint8_t>(x >> (56 - i * 8));
    }
}

void sha_unpack128(uint8_t* str, __uint128_t x)
{
    for (int i = 0; i < 16; ++i)
    {
        str[i] = static_cast<uint8_t>(x >> (120 - i * 8));
    }
}

uint64_t sha_pack64(const uint8_t* str)
{
    uint64_t x = 0;
    for (int i = 0; i < 8; ++i)
    {
        x |= (static_cast<uint64_t>(str[i]) << (56-i*8));
    }
    return x;
}
};

const uint64_t SHA512::sha512_k[80] = //  = uint64
    {0x428a2f98d728ae22, 0x7137449123ef65cd,
     0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
     0x3956c25bf348b538, 0x59f111f1b605d019,
     0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
     0xd807aa98a3030242, 0x12835b0145706fbe,
     0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
     0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
     0x9bdc06a725c71235, 0xc19bf174cf692694,
     0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
     0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
     0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
     0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
     0x983e5152ee66dfab, 0xa831c66d2db43210,
     0xb00327c898fb213f, 0xbf597fc7beef0ee4,
     0xc6e00bf33da88fc2, 0xd5a79147930aa725,
     0x06ca6351e003826f, 0x142929670a0e6e70,
     0x27b70a8546d22ffc, 0x2e1b21385c26c926,
     0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
     0x650a73548baf63de, 0x766a0abb3c77b2a8,
     0x81c2c92e47edaee6, 0x92722c851482353b,
     0xa2bfe8a14cf10364, 0xa81a664bbc423001,
     0xc24b8b70d0f89791, 0xc76c51a30654be30,
     0xd192e819d6ef5218, 0xd69906245565a910,
     0xf40e35855771202a, 0x106aa07032bbd1b8,
     0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
     0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
     0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
     0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
     0x748f82ee5defb2fc, 0x78a5636f43172f60,
     0x84c87814a1f0ab72, 0x8cc702081a6439ec,
     0x90befffa23631e28, 0xa4506cebde82bde9,
     0xbef9a3f7b2c67915, 0xc67178f2e372532b,
     0xca273eceea26619c, 0xd186b8c721c0c207,
     0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
     0x06f067aa72176fba, 0x0a637dc5a2c898a6,
     0x113f9804bef90dae, 0x1b710b35131c471b,
     0x28db77f523047d84, 0x32caab7b40c72493,
     0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
     0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
     0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

SHA512::SHA512(const std::string &raw)
    : hash_{
          0x6a09e667f3bcc908,
          0xbb67ae8584caa73b,
          0x3c6ef372fe94f82b,
          0xa54ff53a5f1d36f1,
          0x510e527fade682d1,
          0x9b05688c2b3e6c1f,
          0x1f83d9abfb41bd6b,
          0x5be0cd19137e2179},
      msg_(raw.begin(), raw.end())
{
    padding_and_append();
    assert(msg_.size() % 128 == 0);
    calculate();
}

std::string SHA512::get_hash() const
{
    std::string hash;
    uint8_t buf[8];
    for (auto ch: hash_)
    {
        sha_unpack64(buf, ch);
        hash.append((char*)buf, 8);
    }
    assert(hash.size() == 64);
    return hash;
}

void SHA512::padding_and_append()
{
    __uint128_t len = msg_.size() * 8;
    msg_.push_back(0x80);
    while (msg_.size() % 128 != 112)
        msg_.push_back(0);
    for (int i = 0; i < 16; ++i)
        msg_.push_back(static_cast<uint8_t>(len >> (120 - i * 8)));
    assert(msg_.size() % 128 == 0);
}

void SHA512::calculate()
{
    uint64_t t1, t2;
    const uint8_t *block;
    std::array<uint64_t, 80> w;
    for (size_t i = 0; i < msg_.size() / 128; ++i)
    {
        auto hash_tmp = hash_;
        block = msg_.data() + i * 128;
        for (int j = 0; j < 16; ++j)
        {
            w[j] = sha_pack64(block + j * 8);
        }
        for (int j = 16; j < 80; ++j)
        {
            w[j] = SHA512_F4(w[j - 2]) + w[j - 7] + SHA512_F3(w[j - 15]) + w[j - 16];
        }
        for (int j = 0; j < 80; ++j)
        {
            t1 = hash_tmp[7] + SHA512_F2(hash_tmp[4]) + SHA2_CH(hash_tmp[4], hash_tmp[5], hash_tmp[6]) + sha512_k[j] + w[j];
            t2 = SHA512_F1(hash_tmp[0]) + SHA2_MAJ(hash_tmp[0], hash_tmp[1], hash_tmp[2]);
            hash_tmp[7] = hash_tmp[6];
            hash_tmp[6] = hash_tmp[5];
            hash_tmp[5] = hash_tmp[4];
            hash_tmp[4] = hash_tmp[3] + t1;
            hash_tmp[3] = hash_tmp[2];
            hash_tmp[2] = hash_tmp[1];
            hash_tmp[1] = hash_tmp[0];
            hash_tmp[0] = t1 + t2;
        }
        for (size_t i = 0; i < 8; ++i)
            hash_[i] += hash_tmp[i];
    }
}
