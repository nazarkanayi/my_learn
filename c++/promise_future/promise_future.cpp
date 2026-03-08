

#include <iostream>
#include <thread>
#include <future>
#include <chrono>


int main()
{
    std::promise<int> prom;

    std::future<int> fut= prom.get_future();

    std::thread thd([promi= std::move(prom)]() mutable
{
      std::this_thread::sleep_for(std::chrono::seconds(3));
      promi.set_value(5);
});

int val = fut.get();

std::cout << "promise val "<< val << std::endl;

thd.join();


}