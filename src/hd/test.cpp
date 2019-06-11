#include "dh.h"

#include <iostream>
#include <cassert>
#include <cstdlib>

#define TEST_HD(q, a)                              \
    {                                              \
        dh d1(q, a);                               \
        dh d2(q, a);                               \
        auto k1 = d1.cal_k(d2.get_y());            \
        auto k2 = d2.cal_k(d1.get_y());            \
        assert(k1 == k2);                          \
        std::cout << k1 << " " << k2 << std::endl; \
    }

int main()
{
    srand(time(0));
    TEST_HD(19, 13);
    TEST_HD(19, 2);
    TEST_HD(19, 15);
}