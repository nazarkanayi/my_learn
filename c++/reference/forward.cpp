
#include <type_traits>
#include <utility>  
#include <iostream>

template <class T> 
T&& my_forward(typename std::remove_reference<T>::type& t)
{
  static_assert(!std::is_lvalue_reference<T>::value,
                "std::forward must not be used to convert an rvalue to an lvalue");

 std::cout << "Forwarding an lvalue reference" << std::endl;
  return static_cast<T&&>(t);
}
template <class T> 
T&& my_forward(typename std::remove_reference<T>::type&& t) noexcept
{
  static_assert(!std::is_rvalue_reference<T>::value,
                "std::forward must not be used to convert an lvalue to an rvalue");

    std::cout << "Forwarding an rvalue reference" << std::endl;
  return static_cast<T&&>(t);
}




int main() {
    // Example usage of forward
    int x = 10;
   const int& y = my_forward<int>(x); // This will cause a static_assert failure
    return 0;
}

