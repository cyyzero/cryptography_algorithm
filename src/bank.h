#ifndef BANK_H
#define BANK_H

#include "RSA.h"
#include <string>

class Bank : public RSA
{
public:
    Bank()
        : RSA(8) { }

    void send_to_customer(const std::string& msg) const;
    void send_to_merchant(const std::string& msg) const;

    void process_order(const std::string& pdo, const std::string& digital_envelope);

    void set_customer_key(const RSA::Key& key)
    {
        customer_key = key;
    }

    void set_merchant_key(const RSA::Key& key)
    {
        merchant_key = key;
    }

private:
    RSA::Key customer_key;
    RSA::Key merchant_key;
};

#endif // BANK_H
