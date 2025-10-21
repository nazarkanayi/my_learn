
#include <iostream>

#include <thread>
#include <mutex>

#include <condition_variable>
#include <queue>

#include <functional>
#include <future>

class Threadpool
{

public:
    Threadpool()
    {
        for (int i = 0; i < max_thread; i++)
        {
            threads[i] = std::move(std::thread(&Threadpool::thread_function, this));
        }
    }

    template <typename F>
    bool push_task(F &&task)
    {
        std::unique_lock<std::mutex> lock{m_lock};

        taskQueue.emplace(std::forward<F>(task));
        lock.unlock();
        cond_var.notify_one();

        return true;
    }


    template <typename F, typename ... Args>
    bool push_task_arg(F &&task,Args&& ... args )
    {
        std::unique_lock<std::mutex> lock{m_lock};
        auto tsk = std::bind(std::forward<F>(task), std::forward<Args>(args)...);
        taskQueue.emplace(std::move(tsk));
        lock.unlock();
        cond_var.notify_one();

        return true;
    }


    template <class F, class... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_lock);
            if (stop_request)
            {
                throw std::runtime_error("submit on stopped ThreadPool");
            }
            taskQueue.emplace([task]()
                              { (*task)(); });
        }
        cond_var.notify_one();
        return res;
    }

    void thread_function(void)
    {
        while (1)
        {
            std::unique_lock<std::mutex> lock{m_lock};

            cond_var.wait(lock, [this]()
                          { return (!(taskQueue.empty()) || stop_request); });

            if (stop_request)
            {
                return;
            }

            auto task = std::move(taskQueue.front());

            taskQueue.pop();

            lock.unlock();

            task();
        }
    }

    ~Threadpool()
    {
        std::unique_lock<std::mutex> lock{m_lock};
        stop_request = 1;
        lock.unlock();
        cond_var.notify_all();

        for (int i = 0; i < max_thread; i++)
        {
            threads[i].join();
        }
    }

private:
    static constexpr int max_thread = 4;

    std::thread threads[max_thread];

    std::mutex m_lock;

    std::condition_variable cond_var;

    std::queue<std::function<void(void)>> taskQueue;

    bool stop_request{false};
};

int main(void)
{
    Threadpool pool{};

    for (int i = 0; i < 1000; i++)
    {
        auto submission_time = std::chrono::steady_clock::now();
        pool.push_task([submission_time]()
                    {
        auto execution_start_time = std::chrono::steady_clock::now(); // Record execution start time
            std::chrono::duration<double, std::milli> delay = execution_start_time - submission_time;
            
       //std::cout << "thread  0x" << std::hex << std::this_thread::get_id()  << std::endl;
       //std::cout << "task " << std::endl;
       //std::this_thread::sleep_for(std::chrono::milliseconds(50));
       std::cout << "Time from submission to execution: " << delay.count() << " ms" << std::endl; });
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50000));
}
