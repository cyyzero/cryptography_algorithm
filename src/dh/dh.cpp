#include <cstdlib>
#include <cmath>
#include <iostream>
#include "dh.h"

namespace
{

uint64_t pow(uint64_t x, uint64_t y, uint64_t q)
{
    if (y == 0)
        return 1;
    auto tmp = pow(uint64_t(__uint128_t(x * x) % q), y/2, q);
    auto tmp2 = uint64_t(__uint128_t(tmp * tmp) % q);
    if (y % 2 == 1)
    {
        return uint64_t(__uint128_t(tmp2 * x) % q);
    }
    else
    {
        return tmp2;
    }
}

bool is_prime(uint64_t num)
{
    if (num <= 1)
    {
        return false;
    }
    auto sqrt = std::sqrt(num);
    for (uint64_t i = 2; i <= sqrt; ++i)
    {
        if (num % i == 0)
        {
            return false; 
        }
    }
    return true;
}

}

dh::dh(uint64_t q, uint64_t a)
    : q_(q), a_(a), x_(random() % q_), y_(pow(a_, x_, q_))
{
}

uint64_t dh::cal_k(uint64_t y)
{
    k_ = pow(y, x_, q_);
    return k_;
}

uint64_t dh::get_k() const
{
    return k_;
}

uint64_t dh::get_y() const
{
    return y_;
}

std::pair<uint64_t, uint64_t> generate_q_a()
{
    int q, a;
    do {
        q = random();
    } while (!is_prime(q));

    return {q, a};
}
