#ifndef BANK_H
#define BANK_H

#include "BigInteger.h"
#include <string>

class Bank
{
public:
    Bank();

    void send_to_customer(const std::string& msg) const;
    void send_to_merchant(const std::string& msg) const;

private:

};

#endif // BANK_H