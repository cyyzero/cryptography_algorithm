#ifndef MERCHANT_H
#define MERCHANT_H

#include <string>

class Merchant
{
public:
    Merchant();

    void send_to_customer(const std::string& msg) const;
    void send_to_bank(const std::string& msg) const;

};

#endif // MERCHANT_H