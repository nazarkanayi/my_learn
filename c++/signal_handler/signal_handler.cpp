
#include <iostream>
#include <csignal>
#include <atomic>
#include <chrono>
#include <thread>

static std::atomic<bool> exit_flag = false;

void signal_handler(int signal)
{
    if(signal == SIGINT)
    {
        std::cout<< "int signal" << "\n";
        exit_flag= true;
    }
}



int main()
{

    std::signal(SIGINT,signal_handler);

    while(!exit_flag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    return 0;
}