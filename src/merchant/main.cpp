#include "merchant.h"
#include <unistd.h>
#include <sys/time.h>
int main()
{
    ::sleep(2);
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    srand(tv.tv_usec);
    Merchant merchant;
    merchant.run();
    ::sleep(2);

}