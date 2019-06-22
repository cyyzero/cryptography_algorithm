#ifndef __HD_H
#define __HD_H

#include <stdint.h>

class dh
{

public:
    dh(uint64_t q, uint64_t a);
    uint64_t cal_k(uint64_t y_);
    uint64_t get_k() const;
    uint64_t get_y() const;
private:
    uint64_t q;
    uint64_t a;
    uint64_t x;
    uint64_t y;
    uint64_t k;
};

#endif // __HD_H