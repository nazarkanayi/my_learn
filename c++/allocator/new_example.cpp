
#include <iostream>

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

    int* Array = new int[20];

    for(int i= 0; i <20;i++)
    {
        std::cout << "Value " << i << " " << Array[i] << std::endl;
    }

    delete[] Array;

    long& Value = *(new long);
    std::cout << "Value " << Value << std::endl;

    delete &Value;


    
}