#include <atomic>
#include <cstddef>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
    #include <pthread.h>
    #include <sched.h>
#include <cstring>

template<typename T>
class SPSCOverwriteRing {
public:
    explicit SPSCOverwriteRing(size_t size)
        : size_(size),
          buffer_(size),
          head_(0),
          tail_(0) {}

    // Producer
    void push(const T& item) {
        uint64_t head = head_.load(std::memory_order_relaxed);
        uint64_t tail = tail_.load(std::memory_order_acquire);

        //std::cout << "producer head: " << head << "\n";
        //std::cout << "producer tail: " << tail << "\n";

        
        // Detect overflow
        if (head - tail >= size_) {
            // Drop oldest
            tail_.store(tail + 1, std::memory_order_release);
            std::cout << "producer overflow\n";
        }

        buffer_[head % size_] = item;

        head_.store(head + 1, std::memory_order_release);
    }

    // Consumer
    bool pop(T& out) {
        uint64_t tail = tail_.load(std::memory_order_relaxed);
        uint64_t head = head_.load(std::memory_order_acquire);

        if (tail == head) {
            return false; // empty
        }

        out = buffer_[tail % size_];

        tail_.store(tail + 1, std::memory_order_release);
        return true;
    }

    // Data loss detection
    bool lost_data() const {
        uint64_t head = head_.load(std::memory_order_acquire);
        uint64_t tail = tail_.load(std::memory_order_acquire);
        return (head - tail) > size_;
    }

private:
    const size_t size_;
    std::vector<T> buffer_;

    std::atomic<uint64_t> head_;  // monotonic (never wraps)
    std::atomic<uint64_t> tail_;  // monotonic (never wraps)
};


int main()
{
    

    SPSCOverwriteRing<int> ring(4);
    
    // Get the number of available CPU cores
    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Number of available CPU cores: " << num_cores << std::endl;

    

    // Consumer: pops and prints until producer finished and buffer drained
    std::thread consumer([&]{
        int value;
       // Get the native handle of the current thread
    pthread_t current_thread = pthread_self();

    // Create a CPU set and clear it
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // Add the desired core to the CPU set
    CPU_SET(1, &cpuset);

    // Set the thread's affinity
    int s = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0) {
        std::cerr << "Error setting thread affinity for core 1: " << strerror(s) << std::endl;
        
    }
    else {
        std::cerr << "Successfully set thread affinity to core 1\n";
    }

        while (1) {

            auto now = std::chrono::steady_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
            //std::cout << "steady_clock: " << ms << " ms\n";
            if (ring.pop(value)) {
                std::cout << "popped: " << value << '\n';
            } else {
               std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
             
        }
    });


    // Producer: pushes numbers into the ring
    std::thread producer([&]{

        // Get the native handle of the current thread
    pthread_t current_thread = pthread_self();

    // Create a CPU set and clear it
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // Add the desired core to the CPU set
    CPU_SET(0, &cpuset);

    // Set the thread's affinity
    int s = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0) {
        std::cerr << "Error setting thread affinity for core 0: " << strerror(s) << std::endl;
        
    }
    else {
        std::cerr << "Successfully set thread affinity to core 0\n";
    }

    
        for (int i = 1; i <= 5000000; ++i) {
            ring.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
    });

    producer.join();
    consumer.join();
    return 0;
}