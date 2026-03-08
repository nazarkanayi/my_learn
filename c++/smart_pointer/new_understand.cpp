


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
    std::printf("new(size_t), size = %zu\n", sz);
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success
 
    if (void *ptr = std::malloc(sz))
        return ptr;
 
    throw std::bad_alloc{}; // required by [new.delete.single]/3
}


template<typename T>
class SmartPointer
{
private:
    T* ptr;
public:
    
   SmartPointer()
   {
      ptr = new(std::nothrow) T();
      if(ptr == nullptr)
      {
         std::cout << "Memory allocation failed\n";
      }
   }

   SmartPointer(const T& val)
   {
      ptr = new(std::nothrow) T(val);
        if(ptr == nullptr)
        {
             std::cout << "Memory allocation failed\n";
        }

   }

   ~SmartPointer()
   {
     delete ptr;
   }
   
   T& GetVal()
   {
      if(ptr == nullptr)
      {
         std::cout << "Null Pointer Exception\n";
         return T();
         
      }
     return *ptr;
   }
};

int main() {

SmartPointer<int> ptr(10);

std::cout << ptr.GetVal() << "\n";



   
}




