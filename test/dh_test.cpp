#include "dh.h"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <unistd.h>

#define TEST_DH(q, a)                              \
    {                                              \
        dh d1(q, a);                               \
        dh d2(q, a);                               \
        auto k1 = d1.cal_k(d2.get_y());            \
        auto k2 = d2.cal_k(d1.get_y());            \
        assert(k1 == k2);                          \
        std::cout << k1 << " " << k2 << std::endl; \
    }


bool check_root(uint64_t q, uint64_t a);

int main()
{
    srand(time(0));
    uint64_t q, a;
    std::pair<uint64_t&, uint64_t&> p(q, a);

    for (int i = 0; i < 10; ++i)
    {
        p = dh::generate_q_a();
        std::cout << "q: " << q << " a: " << a << std::endl;
        TEST_DH(q, a);
        // ::pause();
    }

}