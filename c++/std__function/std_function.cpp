#include <iostream>
#include <functional>

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


int main()
{
    int val[34444] = {};
   std::function<void(int)> func = [=](int num) {
                  std::cout << val << std::endl; 
    };

    func(20);

    std::function<void(int)> func1 ;

    std::function<void(int)> func2;

    func1 = func;

    func1(30);
}