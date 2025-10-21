#include <iostream>
#include <memory>


template <typename T , typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Myclass
{

    public:

    Myclass()
    {
        std::cout << "Myclass constructor" << '\n';
    }

    Myclass(int a) : val{a}
    {
        std::cout << "Myclass constructor with a " << a <<'\n';

    }

    ~Myclass()
    {
        std::cout << "Myclass destructor " << val << '\n';
    }

    int val;

};


int main(void)
{

    
    std::unique_ptr<Myclass> ptr(new Myclass()) ;

    std::unique_ptr<Myclass> my_ptr = my_make_unique<Myclass>(20);

    ptr.reset(new Myclass(10));

}