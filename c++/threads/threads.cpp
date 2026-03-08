


#include <thread>

#include<iostream>

void function(int val)
{
    std::cout << "thread " << val << "\n";
}

int main()
{

    std::thread thd(function,1);
    thd.join();

    std::thread th2([val=2](){
      std::cout << "lampda thread " << val << "\n";  
    });

    th2.join();
}