#include <iostream>
#include <memory>   


template<typename T>
class CircularBuffer {
public:
    CircularBuffer(size_t size)
        : size_(size),
          buffer_(new T[size]),
          head_(0),
          tail_(0) {}

    void push(const T& item) {
        size_t nextHead = (head_ + 1) % size_;

        // First check overflow condition
        if (nextHead == tail_) {
            // buffer full -> advance tail (drop oldest)
            tail_ = (tail_ + 1) % size_;
        }

        // Now safe to copy the data
        buffer_[head_] = item;
        head_ = nextHead;

        std::cout << "head " << head_ << std::endl;
    }

    bool pop(T& item) {
        if (empty())
            return false;

        item = buffer_[tail_];
        tail_ = (tail_ + 1) % size_;

        std::cout << "tail " << tail_ << std::endl;
        return true;
    }

    bool empty() const {
        return head_ == tail_;
    }

    bool full() const {
        return (head_ + 1) % size_ == tail_;
    }

    void print_all() const {
        for (size_t i = tail_,j=0; j < size_; i = (i + 1) % size_,j++) {
            std::cout << buffer_[i] << " ";
        }
        std::cout << std::endl;
    }
private:
    size_t size_;
    std::unique_ptr<T[]> buffer_;
    size_t head_;
    size_t tail_;
};

int main() {
    CircularBuffer<int> ring(10);

    // Example usage
    ring.push(1);
    ring.push(2);

    for(int i = 0; i <= 50; ++i) {
        ring.push(i);
        std::cout << "Pushed: " << i << std::endl;
    }

    ring.print_all();
    int value;
    while (ring.pop(value)) {
        std::cout << "Popped: " << value << std::endl;
    }

    return 0;
}
