


#include <iostream>
#include <string>

void print(std::string&& c)
{
std::cout << &c << std::endl;
std::cout << c << std::endl;
std::string tmp = c;
std::cout << c << std::endl;
std::cout << tmp << std::endl;
}

int main(void)
{
std::string a = "test";

std::string b = "nazar";

//std::string&& c  = a+b;
std::cout << &b << std::endl;

print(a+b);
std::cout << b << std::endl;
    
}
