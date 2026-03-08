

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

std::mutex mtx;

std::condition_variable cv;

std::queue<int> queue;

std::atomic<bool> stop_request{false};

void producer(void)
{
    int val{0};
    for (int i = 0; i < 1000; i++)
    {
        std::unique_lock<std::mutex> lock{mtx};

        cv.wait(lock, []()
                { return (queue.size() < 100); });
        val++;
        queue.push(val);

        cv.notify_one();
    }
}

void consumer(void)
{

    while (!stop_request)
    {
        std::unique_lock<std::mutex> lock{mtx};

        cv.wait(lock, []()
                { return (!queue.empty()); });

        auto val = queue.front();

        queue.pop();

        std::cout << "consumer " << val << std::endl;

        cv.notify_all();
    }
}

int main()
{
    std::thread th1(consumer);

    std::thread th2(producer);

    th1.join();
    th2.join();
}