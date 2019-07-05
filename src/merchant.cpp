#include "merchant.h"
#include "sha.h"
#include  "utility.h"
#include <iostream>
#include <cassert>

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