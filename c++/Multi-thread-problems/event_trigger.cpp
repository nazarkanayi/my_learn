

#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <chrono>


std::mutex mtx;

std::condition_variable cv_evt;

int event =0;

int done =0;

void EevntTriggerThread(void)
{
    for(int i = 0 ; i< 1000 ; i++)
    {
       {
        std::unique_lock<std::mutex> lock{mtx};

       if(event)
       {
        std::cout << "event not processed " << "\n";
       }
       event= 1;

       cv_evt.notify_one();
       }

       std::this_thread::sleep_for(std::chrono::milliseconds(10));
       
    }

    
    std::unique_lock<std::mutex> lock{mtx};
    done =1;
    cv_evt.notify_one();
}


void ProcessThread(void)
{
    int event_count{0};
    while(1)
    {
        std::unique_lock<std::mutex> lock{mtx};
        cv_evt.wait(lock,[](){

            return event==1 || done ;

        });

        if(done)
        break;

        event=0;
        std::cout << "event processed " << ++event_count <<  "\n";

    }
}

int main()
{

    std::thread th1(EevntTriggerThread);

    std::thread th2(ProcessThread);


    th1.join();

    
    th2.join();

    return 0;

}
