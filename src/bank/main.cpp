#include "bank.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
    srand(time(NULL));
        ::sleep(2);
    Bank bank;
    bank.run();
    ::sleep(2);


}