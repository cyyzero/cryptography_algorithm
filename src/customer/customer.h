#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <cstdint>
#include <string>

#include "RSA.h"
#include "info.h"
#include "server.h"
#include "utility.h"
#include <array>

class Customer : public RSA, public Server
{
public:

    Customer();

    void send_to_bank(const std::string& message) const;
    void send_to_merchant(const std::string& message) const;
    void set_merchant_key(const RSA::Key&);
    void set_bank_key(const RSA::Key&);
    void gen_request(const Payment_info& pi, const Order_info& oi) const;

private:
    void send(int port, const std::string& msg) const;

    void on_message1(const std::string& msg);


    void on_message2(const std::string& msg);

    RSA::Key merchant_key;
    RSA::Key bank_key;
};

#endif // CUNTOMER_H
