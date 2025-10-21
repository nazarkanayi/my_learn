#include <vector>
#include <iostream>

int main() {
    // Declare a vector of integers
    std::vector<int> numbers;

    // Add elements to the vector
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    // Access elements
    std::cout << "First element: " << numbers[0] << std::endl;

    // Get the size of the vector
    std::cout << "Vector size: " << numbers.size() << std::endl;

    // Iterate and print elements
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Remove the last element
    numbers.pop_back();
    std::cout << "Vector size after pop_back: " << numbers.size() << std::endl;

    std::abort();

    return 0;
}