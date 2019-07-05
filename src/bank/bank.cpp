#include "bank.h"

#include "sha.h"
#include "des.h"
#include "RSA.h"
#include "utility.h"

#include <cassert>

namespace
{
uint32_t get_int(const char* l)
{
    uint32_t len;
    char* p = (char*)&len;
    for (int i = 0; i < 4; ++i)
    {
        p[i] = l[i];
    }
    return len;
}
};

Bank::Bank()
  : RSA(8), Server(BANK_PORT), ready(false)
{
    set_msg_callback([this] (const std::string& msg) {
        this->on_message1(msg);
    });
    auto key = get_public_key();
    auto key_str = key.first.toString() + " " + key.second.toString();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    send_to_customer(key_str);

}

void Bank::run()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{return ready;});
        process_order();
        ready = false;
    }
}

void Bank::process_order() const
{
    
    uint64_t num = *(uint64_t*)decryptByPrivate(digital_envelope).data();
    std::cout << "ks: " << num << std::endl;
    Block ks(num);

    auto de_pdo = des_decrypt(pdo, ks);
    uint32_t pi_len = get_int(de_pdo.data());
    uint32_t ds_len = get_int(de_pdo.data() + 4);
    uint32_t oi_md_len = get_int(de_pdo.data() + 8);

    std:: cout << "len: " << pi_len << " " << ds_len << " " << oi_md_len << '\n';

    std::string pi_str(de_pdo, 12, pi_len);
    std::string ds(de_pdo, 12 + pi_len, ds_len);
    std::string oi_md(de_pdo, 12 + pi_len + ds_len, oi_md_len);

    OUTPUT_STR(pi_str);
    OUTPUT_STR(ds);
    OUTPUT_STR(oi_md);

    auto pi_md = SHA512(pi_str).get_hash();

    auto po_md1 = RSA::decryptByPublic(ds, customer_key);

    auto po_md2 = SHA512(pi_md + oi_md).get_hash();

    if (po_md1 == po_md2)
    {
        std::cout << "!!! OK  !!!" << std::endl;
    }
    else
    {
        std::cout << "!!! NOT MATCH !!!" << std::endl;
    }
    
}

void Bank::on_message1(const std::string& msg)
{
    auto pos = msg.find_first_of(' ');
    BigInteger e(std::string(msg, 0, pos));
    BigInteger n(std::string(msg, pos + 1));
    customer_key = RSA::Key(e, n);
    std::cout << "Bank recv customer : " << e << " " << n << std::endl;
    set_msg_callback([this] (const std::string& msg) {
        this->on_message2(msg);
    });
}

void Bank::on_message2(const std::string& msg)
{
    pdo = msg;
    set_msg_callback([this] (const std::string& msg) {
        this->on_message3(msg);
    });
}

void Bank::on_message3(const std::string& msg)
{
    digital_envelope = msg;
    set_msg_callback([this] (const std::string& msg) {
        this->on_message2(msg);
    });
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
    }
    cv.notify_one();
}

void Bank::send_to_customer(const std::string& msg) const
{
    send_msg(msg, CUSTOMER_PORT);
}

