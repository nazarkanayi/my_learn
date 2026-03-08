
#include <iostream>



template <typename T>
class UniquePtr
{

    private:

    T* ptr;

    public:

    UniquePtr():ptr{nullptr}
    {}

    UniquePtr(T* lptr):ptr{lptr}
    {}

    UniquePtr(const UniquePtr& other) = delete;

    UniquePtr& operator=(const UniquePtr& other) = delete;

     UniquePtr(UniquePtr&& other) noexcept
     {
       ptr=  other.ptr;
       other.ptr = nullptr;
     }


     UniquePtr& operator=(UniquePtr&& other) noexcept
     {
        if(this != &other)
        {
            delete ptr;
            ptr=  other.ptr;
            other.ptr = nullptr;
        }

        return *this;
     }

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
   UniquePtr<int> ptr{new int};

   int* loptr = ptr.get();

   *loptr =10;

   std::cout << *ptr << std::endl;
}