
#include <thread>
#include <iostream>
#include <chrono>
#include <array>
#include <functional>
#include <memory>
#include <csignal>
#include <atomic>

class Runnable
{
public:
    virtual int Init(void) = 0;
    virtual int Run(std::atomic<bool> &stop_flag) = 0;
    virtual int Shutdown(void) = 0;
};

template <size_t NumberOfRunnable>
class Application
{
public:
    using RunnableFactory = std::function<std::unique_ptr<Runnable>(void)>;
    Application(std::array<RunnableFactory, NumberOfRunnable> factories = {}) : runnable_factories(std::move(factories))
    {
        std::cout << "Application created" << std::endl;
    }

    int InitRunnables(void)
    {
        for (int i = 0; i < NumberOfRunnable; i++)
        {
            runnables[i] = runnable_factories[i]();
            if (runnables[i]->Init() != 0)
            {
                std::cout << "Failed to initialize runnable " << i << std::endl;
                return -1;
            }
        }
        return 0;
    }

    int RunRunnables(std::atomic<bool> &stop_flag)
    {
        for (int i = 0; i < NumberOfRunnable - 1; i++)
        {
            threads[i] = std::thread(&Application::RunSingleRunnable, this, std::ref(runnables[i]), std::ref(stop_flag));
        }

        if (NumberOfRunnable > 0)
        {
            RunSingleRunnable(runnables[NumberOfRunnable - 1], stop_flag);
        }
        for (int i = 0; i < NumberOfRunnable - 1; i++)
        {
            if (threads[i].joinable())
            {
                threads[i].join();
            }
        }

        return 0;
    }

    int ShutdownRunnables(void)
    {
        for (int i = 0; i < NumberOfRunnable; i++)
        {
            if (runnables[i]->Shutdown() != 0)
            {
                std::cout << "Failed to shutdown runnable " << i << std::endl;
                return -1;
            }
        }
        return 0;
    }

    void RunSingleRunnable(std::unique_ptr<Runnable> &runnable, std::atomic<bool> &stop_flag)
    {
        if (runnable->Run(stop_flag) != 0)
        {
            std::cout << "Failed to run runnable " << std::endl;
        }
    }

    ~Application()
    {
        std::cout << "Application destroyed" << std::endl;
    }

private:
    std::array<RunnableFactory, NumberOfRunnable> runnable_factories;
    std::array<std::unique_ptr<Runnable>, NumberOfRunnable> runnables;
    std::array<std::thread, NumberOfRunnable - 1> threads;
};

template <int value>
class MyRunnable : public Runnable
{
public:
    int Init(void) override
    {
        std::cout << "MyRunnable initialized " << value << std::endl;
        return 0;
    }

    int Run(std::atomic<bool> &stop_flag) override
    {
        while (!stop_flag.load())
        {
            /* code */
            std::cout << "MyRunnable running " << value << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return 0;
    }

    int Shutdown(void) override
    {
        std::cout << "MyRunnable shutdown " << value << std::endl;
        return 0;
    }
};

std::atomic<bool> *g_stop_flag = nullptr;

void signal_handler(int signum)
{
    std::cout << "Signal " << signum << " received. Exiting gracefully..." << std::endl;
    if (g_stop_flag)
    {
        g_stop_flag->store(true);
    }
}

int main()
{

    std::atomic<bool> stop_flag{false};
    g_stop_flag = &stop_flag;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::array<Application<3>::RunnableFactory, 3> array = {[]()
                                                            { return std::make_unique<MyRunnable<1>>(); },
                                                            []()
                                                            { return std::make_unique<MyRunnable<2>>(); },
                                                            []()
                                                            { return std::make_unique<MyRunnable<3>>(); }};

    Application<3> app{
        array};
    app.InitRunnables();
    app.RunRunnables(stop_flag);
    app.ShutdownRunnables();

    return 0;
}
