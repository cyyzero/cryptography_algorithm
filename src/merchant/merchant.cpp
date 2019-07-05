#include "merchant.h"
#include "sha.h"
#include  "utility.h"
#include <iostream>
#include <cassert>

Merchant::Merchant()
  : RSA(8), Server(MERCHANT_PORT)
{
    set_msg_callback([this] (const std::string& msg) {
        this->on_message1(msg);
    });
        using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
}

void Merchant::process_customer(const std::string& pdo,
                                const std::string& digital_envelope,
                                const std::string& pi_md,
                                const std::string& oi,
                                const std::string& dual_signature)
{
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

    assert(po_md1 == po_md2);
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
    
}