#include "utility.h"

#include <string>

uint64_t pow(uint64_t x, uint64_t y, uint64_t q)
{
    if (y == 0)
        return 1;
    auto tmp = pow(uint64_t(__uint128_t(x * x) % q), y/2, q);
    // auto tmp2 = Integer(__uint128_t(tmp * tmp) % q);
    if (y % 2 == 1)
    {
        return uint64_t(__uint128_t(tmp * x) % q);
    }
    else
    {
        return tmp;
    }
}

void print_str(const std::string& str)
{
    for (auto ch : str)
    {
        printf("%x", (int)ch & 0xff);
    }
    printf("\n");
}