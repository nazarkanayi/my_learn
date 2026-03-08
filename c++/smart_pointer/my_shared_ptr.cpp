
#include <iostream>

#include <atomic>

template <typename T>
class SharedPtr {
private:
    T* ptr;
    std::atomic<size_t>* ref_count; // Atomic counter for thread safety

    void release() {
        // fetch_sub returns the value BEFORE decrement
        if (ref_count && ref_count->fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete ptr;
            delete ref_count;
        }
    }

public:
    explicit SharedPtr(T* p = nullptr) 
        : ptr(p), ref_count(p ? new std::atomic<size_t>(1) : nullptr) {}

    SharedPtr(const SharedPtr& other) 
        : ptr(other.ptr), ref_count(other.ref_count) {
        if (ref_count) {
            ref_count->fetch_add(1, std::memory_order_relaxed);
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) {
                ref_count->fetch_add(1, std::memory_order_relaxed);
            }
        }
        return *this;
    }

    ~SharedPtr() { release(); }
    
    T* get()
     {
        return ptr;
     }


    T* operator->()
    {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }
};


int main()
{

SharedPtr<int> ptr{new int};

   int* loptr = ptr.get();

   *loptr =10;

   std::cout << *ptr << std::endl;
}