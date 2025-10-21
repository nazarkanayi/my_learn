#include <iostream>


class Base {
public:

    Base() {
        std::cout << "Base Constructor Called" << std::endl;
    }
    virtual void GeeksforGeeks_Print() {
        std::cout << "Base Function" << std::endl;
    }
    virtual ~Base() {
        std::cout << "Base Destructor Called" << std::endl;
    }
};

class Child : public Base {
public:

    Child() {
        std::cout << "Child Constructor Called" << std::endl;
    }
    void GeeksforGeeks_Print() override {
        std::cout << "Child Function" << std::endl;
        
    }
    ~Child() override {
        std::cout << "Child Destructor Called" << std::endl;
    }
};

int main() {
    
    
    Base *b = new Child();
    b->GeeksforGeeks_Print(); // Calls Child's function due to polymorphism
    delete b; // Calls Child's destructor followed by Base's destructor

    return 0;
}