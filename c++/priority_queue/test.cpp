#include <iostream>
#include <queue>
#include <vector>
#include <functional> // Required for std::greater
#include <thread>
#include <chrono>

void* operator new[](std::size_t sz)
{
    std::printf("new[](size_t), size = %zu\n", sz);
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success
 
    if (void *ptr = std::malloc(sz))
        return ptr;
 
    throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void* operator new(std::size_t sz)
{
    std::printf("1) new(size_t), size = %zu\n", sz);
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success
 
    if (void *ptr = std::malloc(sz))
        return ptr;
 
    throw std::bad_alloc{}; // required by [new.delete.single]/3
}

struct test
{
    int a;
    int b;
    test(int x, int y) : a(x), b(y) {
        //std::cout << "test constructor called with a: " << a << ", b: " << b << std::endl;
    }
    ~test() {
       // std::cout << "test destructor called for a: " << a << ", b: " << b << std::endl;
    }
    test(const test& other) : a(other.a), b(other.b) {
       // std::cout << "test copy constructor called for a: " << a << ", b: " << b << std::endl;
    }
    test(test&& other) noexcept : a(other.a), b(other.b) {
       // std::cout << "test move constructor called for a: " << a << ", b: " << b << std::endl;
        other.a = 0; // Reset the moved-from object
        other.b = 0; // Reset the moved-from object
    }
    test& operator=(const test& other) {
       // std::cout << "test copy assignment operator called for a: " << a << ", b: " << b << std::endl;
        if (this != &other) {
            a = other.a;
            b = other.b;
        }
        return *this;
    }
    test& operator=(test&& other) noexcept {
        //std::cout << "test move assignment operator called for a: " << a << ", b: " << b << std::endl;
        if (this != &other) {
            a = other.a;
            b = other.b;
            other.a = 0; // Reset the moved-from object
            other.b = 0; // Reset the moved-from object
        }
        return *this;
    }
};

 struct MyCompare {
        bool operator()(test const & lhs, test const & rhs) {
            // For a max-heap (default behavior), return true if 'a' has lower priority than 'b'
            // For a min-heap, return true if 'a' has higher priority than 'b'
            return lhs.a > rhs.a; // Example: creates a min-heap
        }
    };

int main() {

    
    std::priority_queue<test, std::vector<test>,MyCompare> min_pq; 

    


  unsigned  int count = 0;
while(1)
{
    if(count % 20 == 0)
    {
        std::cout << "Count: " << count << std::endl;
        test t1(1, 2);
        test t2(3, 4);
        min_pq.push(t1);
        min_pq.push(t2);
        
    }
    count++;
    


    

    if (!min_pq.empty()) {
        std::cout << min_pq.top().a << " " << std::endl;;
        min_pq.pop();
    }
    
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

}
    return 0;
}