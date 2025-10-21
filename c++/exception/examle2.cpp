#include <iostream>
#include <exception>

void myTerminateHandler() {
    std::cerr << "Custom terminate handler called." << std::endl;
    std::abort(); // Or other termination logic
}

int main() {
    std::set_terminate(myTerminateHandler);

    //try {
        throw std::runtime_error("An unhandled exception.");
    //} catch (...) {
    //    std::cerr << "catch called." << std::endl;
        // This catch block will not be reached because the terminate handler is called
    //}

    return 0;
}