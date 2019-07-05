#ifndef MERCHANT_H
#define MERCHANT_H

#include <string>
#include "RSA.h"

class Merchant : public RSA
{
public:
    Merchant()
        : RSA(8) { }

    void send_to_customer(const std::string& msg) const;
    void send_to_bank(const std::string& msg) const;

    void process_customer(const std::string& pdo,
                          const std::string& digital_envelope,
                          const std::string& pi_md,
                          const std::string& oi,
                          const std::string& dual_signature);

    void set_customer_key(const RSA::Key& key)
    {
        customer_key = key;
    }
    void set_bank_key(const RSA::Key& key)
    {
        bank_key = key;
    }
private:
    RSA::Key customer_key;
    RSA::Key bank_key;
};

#endif // MERCHANT_H