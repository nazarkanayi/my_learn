#include <iostream>
#include <cstring>

template <typename Allocator = std::allocator<char>>
class MyString
{

public:
    MyString() : len{0}
    {
        std::cout << "MyString  default constructor" << std::endl;
        ptr = Allocator().allocate(1);
        ptr[len] = '\0'; // Null-terminate the string
    }

    MyString(const char *str)
    {
        // Constructor implementation
        std::cout << "MyString constructor called with string: " << std::endl;
        len = strlen(str);

        ptr = Allocator().allocate(len + 1); // Allocate memory for the string
        strcpy(ptr, str);                    // Copy the string into the allocated memory
        ptr[len] = '\0';                     // Null-terminate the string
    }

    MyString(const MyString &other)
    {
        // Copy constructor implementation
        std::cout << "MyString copy constructor called" << std::endl;
        char *tmp_ptr = Allocator().allocate(other.len + 1);
        strcpy(tmp_ptr, other.ptr);

        ptr = tmp_ptr;   // Assign new memory
        len = other.len; // Update length
    }

    MyString(MyString &&other)
    {
        std::cout << "MyString move constructor called" << std::endl;

        ptr = other.ptr;
        len = other.len;
        other.ptr = nullptr;
        other.len = 0;
    }

    MyString &operator=(const MyString &other)
    {
        std::cout << "MyString copy operator= called" << std::endl;
        char *tmp_ptr = Allocator().allocate(other.len + 1);
        strcpy(tmp_ptr, other.ptr);

        if (ptr != nullptr)
        {
            std::cout << "value before deallocate " << (*ptr == '\0' ? "null string" : ptr) << " " << len << std::endl;
            Allocator().deallocate(ptr, len + 1); // Deallocate old memory
        }
        ptr = tmp_ptr;   // Assign new memory
        len = other.len; // Update length

        return *this;
    }

    MyString &operator=(MyString &&other)
    {
        std::cout << "MyString move operator= called" << std::endl;
        
        if (ptr != nullptr)
        {
            std::cout << "value before deallocate " << (*ptr == '\0' ? "null string" : ptr) << " " << len << std::endl;
            Allocator().deallocate(ptr, len + 1); // Deallocate old memory
        }

        ptr = other.ptr;   // Assign new memory
        len = other.len; // Update length

        other.ptr = nullptr;
        other.len =0;

        return *this;
    }

    int find( const char* s, int pos = 0 ) const
    {
        if (pos < 0 || pos >= len)
            return -1;

        const char* result = strstr(ptr + pos, s);
        return result ? result - ptr : -1;
    }

    int rfind( const char* s, int pos = -1 ) const
    {
        if (pos < 0 || pos >= len)
            pos = len - 1;

        const char* result = nullptr;
        for (int i = pos; i >= 0; --i)
        {
            if (strncmp(ptr + i, s, strlen(s)) == 0)
            {
                result = ptr + i;
                break;
            }
        }
        return result ? result - ptr : -1;
    }
    // Declare the global operator<< as a friend function
    template <typename T>
    friend std::ostream &operator<<(std::ostream &os, const MyString<T> &p);

    ~MyString()
    {
        std::cout << "MyString destructor called " << (ptr ? "valid ptr" : "nullptr") << std::endl;
        if (ptr != nullptr)
        {
            // Destructor implementation
            Allocator().deallocate(ptr, len + 1); // Deallocate memory
        }
    }

private:
    char *ptr;
    int len;
};

// Declare the global operator<< as a friend function
template <typename Allocator = std::allocator<char>>
std::ostream &operator<<(std::ostream &os, const MyString<Allocator> &p)
{
    if(p.ptr != nullptr)
    os << p.ptr; // Output the string
    return os;   // Return the output stream
}

using my_string = MyString<>;

int main()
{
    std::cout << "Hello, World!" << std::endl;

    MyString<> myStr("Hello, MyString with Allocator!");
    std::cout << myStr << std::endl; // Output the string using the overloaded

    my_string str = myStr;

    std::cout << str << std::endl;

    my_string new_str;

    new_str = myStr;

    std::cout << new_str << std::endl;

    new_str = std::move(str);

    std::cout << new_str << std::endl;

    return 0;
}