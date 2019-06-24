#include <cstdlib>
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
