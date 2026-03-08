
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <random>
#include <chrono>

std::mutex mtx;
std::condition_variable cv_odd;  // Specifically for Thread B
std::condition_variable cv_even; // Specifically for Thread C
std::queue<int> data_queue;
bool producer_done = false;

void producer(int limit)
{
    for (int i = 1; i <= limit; ++i)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

            // 2. Standard Mersenne Twister 32-bit engine
            std::mt19937 gen(seed);

            // 3. Define the distribution (range [1, 100])
            std::uniform_int_distribution<int> dist(1, 1000);
            auto val= dist(gen);
            data_queue.push(val);
            std::cout << "[A] Pushed: " << val << std::endl;
        }

        // Notify the specific thread based on the number pushed
        if (i % 5 == 0 || i % 3 == 0)
            cv_odd.notify_one();
        else
            cv_even.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        producer_done = true;
    }
    // Final notification to ensure both threads can exit
    cv_odd.notify_all();
    cv_even.notify_all();
}

void consumerOdd()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait specifically for an odd number at the front
        cv_odd.wait(lock, []
                    { return (!data_queue.empty() && ((data_queue.front() % 5 == 0) ||data_queue.front() % 3 == 0 )) || producer_done; });

        if (producer_done && data_queue.empty())
            break;

        if (!data_queue.empty())
        {
            int val = data_queue.front();
            data_queue.pop();
            std::cout << "[B] Consumed Multiple of 5 or 3: " << val << std::endl;
            cv_even.notify_one();
        }
    }
}

void consumerEven()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait specifically for an even number at the front
        cv_even.wait(lock, []
                     { return (!data_queue.empty()) || producer_done; });

        if (producer_done && data_queue.empty())
            break;

        if (!data_queue.empty())
        {
            int val = data_queue.front();
            data_queue.pop();
            std::cout << "[C] Consumed others: " << val << std::endl;
            cv_odd.notify_one();
        }
    }
}

int main()
{
    std::thread tA(producer, 100);
    std::thread tB(consumerOdd);
    std::thread tC(consumerEven);

    tA.join();
    tB.join();
    tC.join();
    return 0;
}
