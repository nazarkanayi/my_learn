
#include <vector>
#include <string>
#include <memory_resource>
#include <iostream>
#include <array>

int main() {
    // Create a monotonic buffer resource
    std::array<char, 1024> buffer;
    std::pmr::monotonic_buffer_resource mbr(buffer.data(), buffer.size());

    // Create a pmr::vector using the monotonic buffer resource
    std::pmr::vector<int> my_pmr_vector(&mbr);

    my_pmr_vector.push_back(10);
    my_pmr_vector.push_back(20);
    my_pmr_vector.push_back(30);

    for (int val : my_pmr_vector) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Create another pmr::vector using the default new_delete_resource
    std::pmr::vector<std::string> my_other_pmr_vector; // Uses default_resource()

    my_other_pmr_vector.push_back("Hello");
    my_other_pmr_vector.push_back("PMR");

    for (const auto& str : my_other_pmr_vector) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    std::abort();
    return 0;
}