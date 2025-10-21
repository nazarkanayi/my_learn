#include <iostream>
#include <optional>





int main()
{
    std::optional<int> val;

    val.emplace(10);

    std::cout << val.value() << std::endl;

     val.reset();
        try
        {
          std::cout << val.value() << std::endl;
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << '\n';
        }

    return 0;
}