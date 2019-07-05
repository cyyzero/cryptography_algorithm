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

void Bank::process_order(const std::string& pdo, const std::string& digital_envelope)
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

    assert(po_md1 == po_md2);
}