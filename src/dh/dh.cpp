#include <cstdlib>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include "dh.h"

namespace
{

uint64_t pow(uint64_t x, uint64_t y, uint64_t q)
{
    if (y == 0)
        return 1;
    auto tmp = pow(uint64_t(__uint128_t(x * x) % q), y/2, q);
    // auto tmp2 = uint64_t(__uint128_t(tmp * tmp) % q);
    if (y % 2 == 1)
    {
        return uint64_t(__uint128_t(tmp * x) % q);
    }
    else
    {
        return tmp;
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

bool check_root(uint64_t q, uint64_t a)
{
    std::unordered_set<uint64_t> set;
    for (uint64_t j = 1; j < q; ++j)
    {
        auto tmp = pow(a, j, q);
        // std::cout << j << "\t" << tmp << std::endl;
        if (set.find(tmp) == set.end())
        {
            set.insert(tmp);
        }
        else
        {
            return false;
        }
    }
    return true;
}

} // namespace

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

std::pair<uint64_t, uint64_t> dh::generate_q_a()
{
    uint64_t q, a = 0;

restart:

    do {
        q = random() % 1000000;
    } while (!is_prime(q));

    std::cout << q << std::endl;

    for (uint64_t i = 2; i < q;++i)
    {
        if (check_root(q, i))
        {
            a = i;
            break;
        }
    }
    if (a == 0)
    {
        goto restart;
    }

    return {q, a};
}
