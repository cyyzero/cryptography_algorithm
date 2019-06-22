#include "sha.h"
#include <cstdio>
int main()
{
    SHA512 t("abc");
    auto hash = t.get_hash();
    for (auto h: hash)
        printf("%lx\n",  h);
}