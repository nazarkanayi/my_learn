// C++ program to demonstrate a program which will throw an
// error
#include <iostream>

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

    myInteger(int v) // parameterized constructor
    {
        cout << "parameterized constructor " << v << std::endl;
        value = v;
    }

    myInteger(myInteger &t)
    {
    	cout << "copy contructor " << value << "  " << t.value << std::endl;
        value = t.value;


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

    myInteger & operator=(const myInteger &obj)
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
	cout << "inside temp" << std::endl;
    return e;
}

int main()
{
    myInteger I1(10);

    myInteger I2(20);

    myInteger I3(I1);

    myInteger I4 = temp(myInteger(45));

    myInteger I5=temp(temp(I1) + temp(I2));

    I1.printvalue();
    I2.printvalue();

    I3.printvalue();

    I3 = I1;

    I3.printvalue();

    return 0;
}
