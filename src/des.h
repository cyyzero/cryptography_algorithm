#ifndef DES_H
#define DES_H

#include <iostream>
#include <bitset>

typedef std::bitset<64> Block;
typedef std::bitset<56> Key;
typedef std::bitset<48> Code;

typedef std::bitset<32> HBlock;
typedef std::bitset<28> HKey;
typedef std::bitset<24> HCode;

enum Method
{
    e,
    d
};

int ip(const Block &block, HBlock &left, HBlock &right);
int des_turn(HBlock &left, HBlock &right, const Code &subkey);
int exchange(HBlock &left, HBlock &right);
int rip(const HBlock &left, const HBlock &right, Block &block);
Code getkey(const unsigned int n, const Block &bkey);
int des(Block &block, Block &bkey, const Method method);

std::string des_encrypt(std::string msg, Block& bkey);
std::string des_decrypt(const std::string& msg, Block& bkey);

#endif // DES_H
