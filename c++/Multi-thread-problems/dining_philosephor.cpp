#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

void philosopher(int id, std::mutex& left_fork, std::mutex& right_fork) {
    while (true) {
        // Thinking
        std::cout << "Philosopher " << id << " is thinking...\n";
        
        {
            // Deadlock-free acquisition of both forks
            std::scoped_lock lock(left_fork, right_fork);
            
            // Eating
            std::cout << "Philosopher " << id << " is eating...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } // Mutexes released automatically here (RAII)
    }
}

int main() {
    const int num_philosophers = 5;
    std::vector<std::mutex> forks(num_philosophers);
    std::vector<std::thread> philosophers;

    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.emplace_back(philosopher, i, 
                                  std::ref(forks[i]), 
                                  std::ref(forks[(i + 1) % num_philosophers]));
    }

    for (auto& t : philosophers) t.join();
    return 0;
}
