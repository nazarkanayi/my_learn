


#include <iostream>
#include <future>


int main()
{
 std::future<int> fut=    std::async([]()
{
    std::cout << "my asysn \n";
    return 2;
});

//auto value =fut.get();

std::cout << "val \n"; // << value;
}