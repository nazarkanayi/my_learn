#include <iostream>
#include <memory>
#include <string>

struct Person {
    std::string name;
    std::weak_ptr<Person> partner; // Strong reference

    Person(std::string n) : name(n) { std::cout << n << " created\n"; }
    ~Person() { std::cout << name << " destroyed\n"; }
};

int main() {
    auto lucy = std::make_shared<Person>("Lucy");
    auto ricky = std::make_shared<Person>("Ricky");

    // Circular reference created here
    lucy->partner = ricky; 
    ricky->partner = lucy; 

    return 0; // Reference counts stay at 1; memory is leaked.
}
