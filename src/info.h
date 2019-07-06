#ifndef INFO_H
#define INFO_H

#include <cstdint>

struct Payment_info
{
    uint64_t comm_id;
    uint64_t money;
};

struct Order_info
{
    uint64_t account_id;
    uint64_t target_account_id;
    uint64_t money;
};

#endif // INFO_H