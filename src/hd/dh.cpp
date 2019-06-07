#include <cstdlib>
#include "dh.h"

namespace
{

uint64_t pow(uint64_t x, uint64_t y, uint64_t q)
{
    if (y == 0)
        return 1;
    auto tmp = pow((x * x) % q, y/2, q);
    if (y % 2 == 1)
    {
        return (((tmp * tmp) % q) * x) % q;
    }
    else
    {
        return (tmp * tmp) % q;
    }
}
}

dh::dh(uint64_t q_, uint64_t a_)
    : q(q_), a(a_), x(random() % q), y(pow(a, x, q))
{
}

uint64_t dh::cal_k(uint64_t y_)
{
    k = pow(y_, x, q);
    return k;
}

uint64_t dh::get_k() const
{
    return k;
}

uint64_t dh::get_y() const
{
    return y;
}


