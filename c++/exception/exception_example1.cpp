#include <iostream>
#include <thread>
 
class foo
{
public:
    foo() { std::cerr << "foo::foo()\n"; }
    ~foo() { std::cerr << "foo::~foo()\n"; }
};
 
// Static object, expecting destructor on exit
foo f;
 
void run()
{
    ////throw std::runtime_error("Thread failure");
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::terminate();
    }
}
 
int main()
{
    std::set_terminate([]()
    {
        try
        {
            std::exception_ptr eptr{std::current_exception()};
            if (eptr)
            {
                std::rethrow_exception(eptr);
            }
            else
            {
                std::cerr << "Exiting without exception\n";
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "Unknown exception caught\n";
        }
        std::exit(EXIT_FAILURE);
    });
 
    std::thread t{run};

    while(1)
    {
        std::cerr << "running\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
    }
    t.join();

    
}