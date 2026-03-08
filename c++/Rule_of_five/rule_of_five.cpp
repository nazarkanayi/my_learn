#include <iostream>
#include <cstring>

class Buffer {
private:
    char* data;
    size_t size;

public:
    // 0. Regular Constructor
    Buffer(size_t s) : size(s), data(new char[s]) {
        std::cout << "Constructed\n";
    }

    // 1. Destructor - Cleans up the memory
    ~Buffer() {
        delete[] data;
        std::cout << "Destructed\n";
    }

    // 2. Copy Constructor - Creates a "deep copy" of the data
    Buffer(const Buffer& other) : size(other.size), data(new char[other.size]) {
        memcpy(data, other.data, size );
        std::cout << "Copy Constructor\n";
    }

    // 3. Copy Assignment Operator - Replaces data with a deep copy
    Buffer& operator=(const Buffer& other) {
        if (this != &other) { // Self-assignment check
            delete[] data;    // Delete old data
            size = other.size;
            data = new char[size];
            memcpy(data, other.data, size * sizeof(char));
        }
        std::cout << "Copy Assignment\n";
        return *this;
    }

    // 4. Move Constructor - "Steals" the pointer from a temporary object
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr; // Leave original in a safe, empty state
        other.size = 0;
        std::cout << "Move Constructor\n";
    }

    // 5. Move Assignment Operator - Transfers ownership to an existing object
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;    // Clean up current resource
            data = other.data; // Steal the new one
            size = other.size;
            other.data = nullptr; // Reset original
            other.size = 0;
        }
        std::cout << "Move Assignment\n";
        return *this;
    }

    bool IsValid()
    {
        return data!=nullptr;
    }

    char* GetData()
    {
       return data;
    }
};


int main()
{

    Buffer buf(5);

    Buffer buf2{buf};

    Buffer buf3{std::move(buf)};

    
}