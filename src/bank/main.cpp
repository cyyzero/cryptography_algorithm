#include "bank.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
int main()
{
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    srand(tv.tv_usec);
        ::sleep(2);
    Bank bank;
    bank.run();
}