#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class BoundedBuffer {
    std::queue<int> buffer;
    size_t capacity;
    std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;

public:
    BoundedBuffer(size_t cap) : capacity(cap) {}

    void produce(int item) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait if buffer is full
        not_full.wait(lock, [this]() { return buffer.size() < capacity; });

        buffer.push(item);
        std::cout << "Produced: " << item << " | Size: " << buffer.size() << "\n";

        // Notify consumers that data is available
        not_empty.notify_one();
    }

    int consume() {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait if buffer is empty
        not_empty.wait(lock, [this]() { return !buffer.empty(); });

        int item = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << item << " | Size: " << buffer.size() << "\n";

        // Notify producers that space is available
        not_full.notify_one();
        return item;
    }
};

void producerTask(BoundedBuffer& bb) {
    for (int i = 1; i <= 10; ++i) bb.produce(i);
}

void consumerTask(BoundedBuffer& bb) {
    for (int i = 1; i <= 10; ++i) bb.consume();
}

int main() {
    BoundedBuffer bb(3); // Small capacity to force waiting
    std::thread p(producerTask, std::ref(bb));
    std::thread c(consumerTask, std::ref(bb));

    p.join();
    c.join();
    return 0;
}
