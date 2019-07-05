#include "merchant.h"
#include "sha.h"
#include  "utility.h"
#include <iostream>
#include <cassert>
#include <thread>

Merchant::Merchant()
  : RSA(8), Server(MERCHANT_PORT), ready(false)
{
    set_msg_callback([this] (const std::string& msg) {
        this->on_message1(msg);
    });
        using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
}

void Merchant::run()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{return ready;});
        process_customer();
        ready = false;
    }
}

void Merchant::process_customer() const
{
    send_to_bank(pdo);
    send_to_bank(digital_envelope);
    auto po_md1 = RSA::decryptByPublic(dual_signature, customer_key);

    auto oi_md = SHA512(oi).get_hash();
    auto po_md2 = SHA512(pi_md + oi_md).get_hash();

#define OUTPUT_STR(var)\
    std::cout << #var " : "; \
    print_str(var);

    OUTPUT_STR(oi);
    OUTPUT_STR(oi_md);
    OUTPUT_STR(pi_md);
    OUTPUT_STR(po_md1);
    OUTPUT_STR(po_md2);

    if (po_md1 == po_md2)
    {
        std::cout << "!!! OK  !!!" << std::endl;
    }
    else
    {
        std::cout << "!!! NOT MATCH !!!" << std::endl;
    }
    
}

void Merchant::send_to_customer(const std::string& msg) const
{
    send_msg(msg, CUSTOMER_PORT);
}

void Merchant::send_to_bank(const std::string& msg) const
{
    send_msg(msg, BANK_PORT);
}

void Merchant::on_message1(const std::string& msg)
{
    auto pos = msg.find(' ');
    BigInteger e(std::string(msg, 0, pos));
    BigInteger n(std::string(msg, pos + 1));
    customer_key = RSA::Key(e, n);
    std::cout << "Merchant recv customer : " << e << " " << n << std::endl;
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message2(msg);
    });
}

void Merchant::on_message2(const std::string& msg)
{
    pdo = msg;
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message3(msg);
    });
}

void Merchant::on_message3(const std::string& msg)
{
    digital_envelope = msg;
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message4(msg);
    });
}

void Merchant::on_message4(const std::string& msg)
{
    pi_md = msg;
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message5(msg);
    });
}

void Merchant::on_message5(const std::string& msg)
{
    oi = msg;
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message6(msg);
    });
}

void Merchant::on_message6(const std::string& msg)
{
    dual_signature = msg;
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message2(msg);
    });
    {
        std::lock_guard<std::mutex> lg(m);
        ready = true;
    }
    cv.notify_one();
}
