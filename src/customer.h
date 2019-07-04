#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <cstdint>
#include <string>

class Customer
{
public:
    struct Payment_info
    {
        uint64_t id;

    };

    struct Order_info
    {
        uint64_t id;

    };

    Customer();

    void send_to_bank(const std::string& message) const;
    void send_to_merchant(const std::string& message) const;

};

#endif // CUNTOMER_H