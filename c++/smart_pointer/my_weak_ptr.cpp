#include <iostream>
#include <atomic>

// --- Control Block ---
// Tracks both strong and weak references to manage lifetimes.
struct ControlBlock {
    std::atomic<size_t> strong_count;
    std::atomic<size_t> weak_count;

    ControlBlock(size_t s, size_t w) : strong_count(s), weak_count(w) {}
};

// Forward declaration for WeakPtr
template <typename T> class WeakPtr;

// --- SharedPtr Implementation ---
template <typename T>
class SharedPtr {
private:
    T* ptr;
    ControlBlock* cb;

    // Friend class to allow WeakPtr to access cb and this constructor
    friend class WeakPtr<T>;

    // Private constructor used by WeakPtr::lock()
    SharedPtr(T* p, ControlBlock* c) : ptr(p), cb(c) {
        if (cb) {
            cb->strong_count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    void release() {
        if (cb) {
            // Decrement strong count; if it reaches 0, destroy the object
            if (cb->strong_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete ptr;
                ptr = nullptr;
                // If no weak pointers exist, delete the control block too
                if (cb->weak_count.load(std::memory_order_acquire) == 0) {
                    delete cb;
                    cb = nullptr;
                }
            }
        }
    }

public:
    // Default/Standard Constructor
    explicit SharedPtr(T* p = nullptr) 
        : ptr(p), cb(p ? new ControlBlock(1, 0) : nullptr) {}

    // Copy Constructor
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), cb(other.cb) {
        if (cb) cb->strong_count.fetch_add(1, std::memory_order_relaxed);
    }

    // Move Constructor
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), cb(other.cb) {
        other.ptr = nullptr;
        other.cb = nullptr;
    }

    // Copy Assignment
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cb = other.cb;
            if (cb) cb->strong_count.fetch_add(1, std::memory_order_relaxed);
        }
        return *this;
    }

    // Move Assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cb = other.cb;
            other.ptr = nullptr;
            other.cb = nullptr;
        }
        return *this;
    }

    ~SharedPtr() { release(); }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
    size_t use_count() const { return cb ? cb->strong_count.load() : 0; }
    explicit operator bool() const { return ptr != nullptr; }
};

// --- WeakPtr Implementation ---
template <typename T>
class WeakPtr {
private:
    T* ptr;
    ControlBlock* cb;

public:
    WeakPtr() : ptr(nullptr), cb(nullptr) {}

    // Construct from SharedPtr
    WeakPtr(const SharedPtr<T>& s) : ptr(s.ptr), cb(s.cb) {
        if (cb) cb->weak_count.fetch_add(1, std::memory_order_relaxed);
    }

    // Copy Constructor
    WeakPtr(const WeakPtr& other) : ptr(other.ptr), cb(other.cb) {
        if (cb) cb->weak_count.fetch_add(1, std::memory_order_relaxed);
    }

    ~WeakPtr() {
        if (cb) {
            // Decrement weak count
            if (cb->weak_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                // If strong count is already 0, weak pointer is the last to touch the CB
                if (cb->strong_count.load(std::memory_order_acquire) == 0) {
                    delete cb;
                }
            }
        }
    }

    // Attempt to convert to SharedPtr
    SharedPtr<T> lock() const {
        if (!cb) return SharedPtr<T>(nullptr);

        size_t count = cb->strong_count.load(std::memory_order_relaxed);
        while (count != 0) {
            // Ensure object isn't deleted while we try to increment
            if (cb->strong_count.compare_exchange_weak(count, count + 1, 
                std::memory_order_acq_rel, std::memory_order_relaxed)) {
                // We successfully incremented the strong count!
                // We use a specific constructor that doesn't re-increment.
                // For simplicity here, we rely on the SharedPtr(T*, CB*) constructor 
                // which increments again, so we actually need to decrement one back 
                // or create a internal "unchecked" constructor.
                
                // Correction: Use our internal constructor logic manually
                SharedPtr<T> temp;
                temp.ptr = this->ptr;
                temp.cb = this->cb;
                // Since we already incremented 'count' via CAS, we don't need to do it again
                return temp;
            }
        }
        return SharedPtr<T>(nullptr);
    }

    bool expired() const { return !cb || cb->strong_count.load() == 0; }
};


