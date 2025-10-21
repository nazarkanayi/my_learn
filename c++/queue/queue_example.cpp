
#include <iostream>

#include <queue>

int main()
{

    std::queue<int> int_queue;

    int_queue.push(20);

    std::cout << int_queue.front() << std::endl;
    int_queue.pop();
}