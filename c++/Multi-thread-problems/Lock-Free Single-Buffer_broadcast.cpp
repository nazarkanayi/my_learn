#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

// Large data object to demonstrate zero-copy broadcast
struct Message {
    int id;
    long timestamp;
};

template <size_t Size>
class LockFreeDisruptor {
    static_assert((Size & (Size - 1)) == 0, "Size must be a power of 2");

    struct Slot {
        Message msg;
        // Tracks the "version" of the slot to signal it's ready for reading
        std::atomic<int64_t> sequence{-1}; 
    };

    Slot buffer[Size];
    
    // Cache-line aligned producer cursor to prevent false sharing
    alignas(64) std::atomic<int64_t> producer_cursor{0};

public:
    // Producer: Writes to the buffer
    void publish(int id) {
        int64_t seq = producer_cursor.load(std::memory_order_relaxed);
        size_t index = seq & (Size - 1);

        // In a real system, we'd check if we're overwriting a slow consumer here
        
        buffer[index].msg.id = id;
        buffer[index].msg.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

        // Release: ensures data is written BEFORE the sequence is updated
        buffer[index].sequence.store(seq, std::memory_order_release);
        
        producer_cursor.store(seq + 1, std::memory_order_relaxed);
    }

    // Consumer: Reads its own sequence from the shared buffer
    bool try_consume(int64_t& next_to_read, Message& out) {
        size_t index = next_to_read & (Size - 1);
        
        // Acquire: ensures we see the data written by the producer
        if (buffer[index].sequence.load(std::memory_order_acquire) == next_to_read) {
            out = buffer[index].msg; // Shallow copy of the message
            next_to_read++;
            return true;
        }
        return false;
    }
};

void consumerFunc(int id, LockFreeDisruptor<1024>& disruptor) {
    int64_t my_next_seq = 0;
    Message msg;
    int count = 0;

    while (count < 10) {
        if (disruptor.try_consume(my_next_seq, msg)) {
            std::cout << "Consumer " << id << " read ID: " << msg.id << "\n";
            count++;
        } else {
            // Busy-wait or yield to prevent 100% CPU usage in an interview demo
            std::this_thread::yield(); 
        }
    }
}

int main() {
    LockFreeDisruptor<1024> ring;
    
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back(consumerFunc, i, std::ref(ring));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (int i = 0; i < 10; ++i) {
        std::cout << "[Producer] Publishing: " << i << "\n";
        ring.publish(i);
    }

    for (auto& t : consumers) t.join();
    return 0;
}
