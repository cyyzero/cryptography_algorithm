#ifndef MERCHANT_H
#define MERCHANT_H

#include <mutex>
#include <condition_variable>
#include <string>
#include "RSA.h"
#include "server.h"

class Merchant : public RSA, public Server
{
public:
    Merchant();
    
    void run();
    void process_customer() const;

    void set_customer_key(const RSA::Key& key)
    {
        customer_key = key;
    }
    void set_bank_key(const RSA::Key& key)
    {
        bank_key = key;
    }

private:

    void send_to_customer(const std::string& msg) const;
    void send_to_bank(const std::string& msg) const;
    // set customer_key
    void on_message1(const std::string& msg);

    // set pdo
    void on_message2(const std::string& msg);
    // set digital_envelope
    void on_message3(const std::string& msg);
    // set pi_md
    void on_message4(const std::string& msg);
    // set pi
    void on_message5(const std::string& msg);
    // set dual_signature
    void on_message6(const std::string& msg);

    std::mutex m;
    std::condition_variable cv;
    bool ready;
    
    std::string pdo;
    std::string digital_envelope;
    std::string pi_md;
    std::string oi;
    std::string dual_signature;

    RSA::Key customer_key;
    RSA::Key bank_key;
};

#endif // MERCHANT_H