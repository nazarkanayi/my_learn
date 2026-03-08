

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
int count = 1;
const int MAX = 10;

void printOdd() {
    while (count < MAX) {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until count is odd
        cv.wait(lock, []() { return count % 2 != 0; });
        
        std::cout << "Odd: " << count << std::endl;
        count++;
        
        cv.notify_all(); // Wake up the other thread
    }
}

void printEven() {
    while (count <= MAX) {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until count is even
        cv.wait(lock, []() { return count % 2 == 0; });
        
        std::cout << "Even: " << count << std::endl;
        count++;
        
        cv.notify_all(); // Wake up the other thread
    }
}

int main() {
    std::thread t1(printOdd);
    std::thread t2(printEven);

    t1.join();
    t2.join();

    return 0;
}
