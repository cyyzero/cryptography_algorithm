#include "customer.h"
#include "sha.h"
#include "des.h"

#include "utility.h"
#include "utility.h"
#include <cassert>
namespace
{
void str_append_int(std::string& str, uint32_t len)
{
    char*p = (char*)&len;
    for (int i = 0; i < 4; ++i)
    {
        str.push_back(p[i]);
    }
}
};


Customer::Customer()
    : RSA(8), Server(6666)
{
    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message1(msg);
    });

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    auto key = get_public_key();
    auto key_str = key.first.toString() + " " + key.second.toString();
    send_to_bank(key_str);
    send_to_merchant(key_str);
}

void Customer::gen_request(const Payment_info& pi,
                           const Order_info& oi) const
{
    std::string pi_str(reinterpret_cast<const char*>(&pi), sizeof(pi));
    std::string oi_str(reinterpret_cast<const char*>(&oi), sizeof(oi));
    auto pi_md = SHA512(pi_str).get_hash();
    auto oi_md = SHA512(oi_str).get_hash();

    auto po_md = SHA512(pi_md + oi_md).get_hash();
    auto dual_signature = encryptByPrivate(po_md);
    auto po_md1 = decryptByPublic(dual_signature);
    
    // assert(po_md == decryptByPublic(dual_signature));
    uint64_t num = random();
    Block ks(num);
    std::cout << "ks1: " << num << std::endl;
    std::string ks_str((const char*)&num, 8);

    std::string prefix_len;
    uint32_t pi_str_len = pi_str.size();
    uint32_t dual_signature_len = dual_signature.size();
    uint32_t oi_md_len = oi_md.size();
    std:: cout << "len: " << pi_str_len << " " << dual_signature_len << " " << oi_md_len << '\n';
    str_append_int(prefix_len, pi_str_len);
    str_append_int(prefix_len, dual_signature_len);
    str_append_int(prefix_len, oi_md_len);

    auto pdo = des_encrypt(prefix_len + pi_str + dual_signature + oi_md, ks);
    auto digital_envelope = RSA::encryptByPublic(ks_str, bank_key);

    OUTPUT_STR(oi_str);
    OUTPUT_STR(pi_str);
    OUTPUT_STR(pi_md);
    OUTPUT_STR(oi_md);
    OUTPUT_STR(po_md);

    std::cout << "send pdo" << std::endl;
    send_to_merchant(pdo);
    std::cout << "send de" << std::endl;
    send_to_merchant(digital_envelope);
    std::cout << "send pi_md" << std::endl;
    send_to_merchant(pi_md);
    std::cout << "send oi" << std::endl;
    send_to_merchant(oi_str);
    std::cout << "send ds" << std::endl;
    send_to_merchant(dual_signature);
}

void Customer::send_to_bank(const std::string& message) const
{
    send_msg(message, BANK_PORT);
}

void Customer::send_to_merchant(const std::string& message) const
{
    send_msg(message, MERCHANT_PORT);
}

void Customer::set_merchant_key(const RSA::Key& key)
{
    merchant_key = key;
}

void Customer::set_bank_key(const RSA::Key& key)
{
    bank_key = key;
}

void Customer::on_message1(const std::string& msg)
{
    auto pos = msg.find(' ');
    BigInteger e(std::string(msg, 0, pos));
    BigInteger n(std::string(msg, pos + 1));
    bank_key = RSA::Key(e, n);
    std::cout << "Customer recv bank : " << e << " " << n << std::endl;

    Server::set_msg_callback([this] (const std::string& msg) {
        this->on_message2(msg);
    });
}

void Customer::on_message2(const std::string& msg)
{
    
}
