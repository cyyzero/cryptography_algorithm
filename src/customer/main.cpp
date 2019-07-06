#include "customer.h"
#include "sys/time.h"
#include "info.h"

#include <thread>
#include <unistd.h>

int main()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    srand(tv.tv_usec);
    Customer customer;
    std::this_thread::sleep_for(2s);
    
    Payment_info pi;
    pi.comm_id = 1;
    pi.money = 1000;

    Order_info oi;
    oi.account_id = 1;
    oi.money = 1000;

    customer.gen_request(pi, oi);
    std::this_thread::sleep_for(2s);
}