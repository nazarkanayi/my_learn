#include <iostream>
#include <memory>


template <typename T , typename... Args>
std::shared_ptr<T> my_make_shared(Args&&... args)
{
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
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

    
    std::shared_ptr<Myclass> ptr(new Myclass()) ;

    std::shared_ptr<Myclass> my_ptr = my_make_shared<Myclass>(20);

    std::cout << "Use count " << my_ptr.use_count() << std::endl;

}