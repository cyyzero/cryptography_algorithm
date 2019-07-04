#include "rc4.h"

#include <cstdio>

int main(int argc, char *argv[])
{
    int size = 0;

    char source[] = "hellonetworksecurity";
    char pass[] = "123456";
    char *result1 = NULL;
    char *result2 = NULL;

    result1 = Encrypt(source, pass);

    printf("result1=%s\n", result1);

    result2 = Decrypt(result1, pass);

    printf("result2=%s\n", result2);

    return 0;
}
