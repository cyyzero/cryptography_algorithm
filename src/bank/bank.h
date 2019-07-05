#ifndef BANK_H
#define BANK_H

#include "RSA.h"
#include <string>

#include "server.h"

class Bank : public RSA, public Server
{
public:
    Bank();

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

    // set customer_key
    void on_message1(const std::string& msg);
    // set pdo
    void on_message2(const std::string& msg);
    // set digital_envelope
    void on_message3(const std::string& msg);


    std::string pdo;
    std::string digital_envelope;

    RSA::Key customer_key;
    RSA::Key merchant_key;
};

#endif // BANK_H
