#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>
#include <string>

#define CUSTOMER_PORT 6666
#define MERCHANT_PORT (CUSTOMER_PORT + 1)
#define BANK_PORT (CUSTOMER_PORT + 2)

uint64_t pow(uint64_t x, uint64_t y, uint64_t q);
void print_str(const std::string& str);

#define OUTPUT_STR(var)\
    std::cout << #var " : "; \
    print_str(var);

void send_msg(const std::string& msg, int port);

#endif // UTILITY_H
