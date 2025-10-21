#include <mutex>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>
using namespace std;
template <typename T>
class queue
{
private:
    std::mutex              d_mutex;
    std::condition_variable d_condition;
    std::deque<T>           d_queue;
public:
    void push(T const& value) {
        {
            std::unique_lock<std::mutex> lock(this->d_mutex);
            d_queue.push_front(value);
        }
        this->d_condition.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(this->d_mutex);
        this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
        T rc(std::move(this->d_queue.back()));
        this->d_queue.pop_back();
        return rc;
    }
};




int main(void)
{
    queue<std::string> strQ;
    std::promise<void> ready_promise;
    std::shared_future<void> ready_future(ready_promise.get_future());
    int i=0;
    auto lambda = [&,future = ready_future]() { 
        while (future.wait_for(std::chrono::milliseconds(1)) != std::future_status::ready)
    {
        cout << " thread1-" << strQ.pop() << std::endl;
    }
     };

     auto lambda1 = [&,future = ready_future]() { 

        do
    {
        strQ.push("nazar-"+ std::to_string(i++));
    }
    while (future.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready);
     };

    // Create and execute the thread
    

    std::thread thread1(lambda1); 
    std::thread thread(lambda); // Pass 10 to the lambda expression
    std::this_thread::sleep_for(std::chrono::milliseconds(30000));

    ready_promise.set_value();
    cout << "main exit started" << std::endl;
    strQ.push("nazar-exit");
    thread.join();

    thread1.join();




    
}