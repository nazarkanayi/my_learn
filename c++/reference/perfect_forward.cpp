#include <iostream>
#include <type_traits> // For std::is_lvalue_reference_v

template <typename T>
void process_value(T&& val) { // Universal reference to deduce lvalue/rvalue
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "Processing an lvalue reference: " << val << std::endl;
        // Specific logic for lvalue references, e.g., modifying the original object
        if constexpr (std::is_const_v<std::remove_reference_t<T>>)
        {
          
        }
        else
        {
            val =100;
        }
        
    } else {
        std::cout << "Processing an rvalue (or non-reference type): " << val << std::endl;
        // Specific logic for rvalues, e.g., moving or copying
    }

}

int main() {
    int x = 42;
    process_value(x); // T will be 'int&', std::is_lvalue_reference_v<int&> is true
    std::cout << "x after lvalue processing: " << x << std::endl;

    process_value(5); // T will be 'int', std::is_lvalue_reference_v<int> is false
    
    const int y = 20;
    process_value(y); // T will be 'const int&', std::is_lvalue_reference_v<const int&> is true
    
    return 0;
}