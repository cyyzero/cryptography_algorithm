#include "merchant.h"
#include <unistd.h>

int main()
{
    srand(time(NULL));
        ::sleep(2);
    Merchant merchant;
    merchant.run();
    ::sleep(2);

}