#ifndef __HD_H
#define __HD_H

#include <stdint.h>
#include <utility>

class dh
{
public:
    dh(uint64_t q, uint64_t a);
    uint64_t cal_k(uint64_t y);
    uint64_t get_k() const;
    uint64_t get_y() const;
private:
    uint64_t q_;
    uint64_t a_;
    uint64_t x_;
    uint64_t y_;
    uint64_t k_;
};

std::pair<uint64_t, uint64_t> generate_q_a();

#endif // __HD_H