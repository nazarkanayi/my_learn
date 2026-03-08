
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

// A simulated "Large Object"
struct LargeData {
    std::vector<double> matrix;
    int id;
    LargeData(int i) : id(i), matrix(1000000, 0.0) { // ~8MB per object
        std::cout << "[Alloc] LargeData " << id << " created\n";
    }
    ~LargeData() { std::cout << "[Free] LargeData " << id << " destroyed\n"; }
};

class Subscriber {
    std::queue<std::shared_ptr<LargeData>> q;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

public:
    void push(std::shared_ptr<LargeData> data) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(data);
        }
        cv.notify_one();
    }

    bool pop(std::shared_ptr<LargeData>& data) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !q.empty() || done; });
        if (q.empty() && done) return false;
        data = q.front();
        q.pop();
        return true;
    }

    void close() {
        { std::lock_guard<std::mutex> lock(mtx); done = true; }
        cv.notify_all();
    }
};

void producerTask(std::vector<std::shared_ptr<Subscriber>>& subs) {
    for (int i = 1; i <= 3; ++i) {
        // Create one instance, shared across all subscribers
        auto data = std::make_shared<LargeData>(i); 
        for (auto& s : subs) s->push(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    for (auto& s : subs) s->close();
}

void consumerTask(int id, std::shared_ptr<Subscriber> sub) {
    std::shared_ptr<LargeData> data;
    while (sub->pop(data)) {
        std::cout << "Consumer " << id << " using LargeData " << data->id 
                  << " (Refs: " << data.use_count() << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
    }
}

int main() {
    std::vector<std::shared_ptr<Subscriber>> subs = {
        std::make_shared<Subscriber>(),
        std::make_shared<Subscriber>()
    };
    
    std::vector<std::thread> consumers;
    for (int i = 0; i < subs.size(); ++i) 
        consumers.emplace_back(consumerTask, i, subs[i]);

    std::thread p(producerTask, std::ref(subs));
    p.join();
    for (auto& c : consumers) c.join();

    return 0;
}
