#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <cxxabi.h> // For GCC/Clang specific demangling

// Platform-specific demangling function (GCC/Clang specific)
std::string demangle(const char* name) {
    int status = -4; // some arbitrary value to eliminate the compiler warning
    char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
    const std::string ret(status == 0 ? res : name);
    free(res);
    return ret;
}

// Helper to get a human-readable type name, handling const, volatile, and references
template <typename T>
std::string type_name() {
    typedef typename std::remove_reference<T>::type TR;
    std::string r = demangle(typeid(TR).name());
    if (std::is_const<TR>::value) r += " const";
    if (std::is_volatile<TR>::value) r += " volatile";
    if (std::is_lvalue_reference<T>::value) r += "&";
    else if (std::is_rvalue_reference<T>::value) r += "&&";
    return r;
}

// The template function where we want to print the deduced type T
template <typename T>
void func(T&& param) {
    std::cout << "Deduce type T: " << type_name<T>() << std::endl;
    // You can also inspect the type of the parameter itself (param)
    std::cout << "Parameter type: " << type_name<decltype(param)>() << std::endl;
}

int main() {
    int i = 0;
    const int ci = 0;
    const volatile int cvi = 0;

    std::cout << "Calling func with int i:" << std::endl;
    func(i); // T is deduced as int& due to universal reference rules

    std::cout << "\nCalling func with const int ci:" << std::endl;
    func(ci); // T is deduced as const int&

    std::cout << "\nCalling func with rvalue (int literals):" << std::endl;
    func(42); // T is deduced as int

    return 0;
}
