#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>
#include <string>

uint64_t pow(uint64_t x, uint64_t y, uint64_t q);
void print_str(const std::string& str);

#define OUTPUT_STR(var)\
    std::cout << #var " : "; \
    print_str(var);

#endif // UTILITY_H
