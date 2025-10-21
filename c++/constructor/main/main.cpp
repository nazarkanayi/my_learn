// C++ program to demonstrate a program which will throw an
// error
#include <iostream>
#include <memory>

using namespace std;

class myInteger
{
private:
    int value;

public:
    myInteger()
    {
        cout << "default constructor " << std::endl;
    }

    explicit myInteger(int v) // parameterized constructor
    {
        cout << "parameterized constructor " << v << std::endl;
        value = v;
    }

    myInteger(myInteger &t)
    {
        value = t.value;

        cout << "copy contructor " << value << std::endl;
    }
    myInteger(myInteger &&obj)
        : value{obj.value}
    {
        // Nulling out the pointer to the temporary data
        obj.value = 0;
        cout << "move contructor " << value << std::endl;
    }
    ~myInteger()
    {
        cout << "destructor " << value << std::endl;
    }

    myInteger &operator=(const myInteger &obj)
    {
        cout << "operator= " << value << "=" << obj.value << std::endl;
        value = obj.value;
        return *this;
    }
    myInteger operator+(myInteger const &obj)
    {
        cout << "operator+ " << value << "+" << obj.value << std::endl;
        myInteger res;
        res.value = value + obj.value;

        return res;
    }

    void printvalue(void)
    {
        cout << value << std::endl;
    }

    //...other things in class
};

myInteger temp(myInteger e)
{
    return e;
}

int main()
{
    std::unique_ptr<myInteger> v1 = std::make_unique<myInteger>(1);
    std::unique_ptr<myInteger> v2 = std::make_unique<myInteger>();
    std::unique_ptr<myInteger> v3 = std::make_unique<myInteger>(2);
    *v2=*v1+*v3;

    v2->printvalue();



    return 0;
}
