


#include <iostream>
#include <future>


int main()
{


    std::packaged_task<int(void)> task([]()
{
  std::cout << "task \n";

return 5;
});

 auto fut =task.get_future();
std::thread thd(std::move(task));

auto val = fut.get();

std::cout << "val "<< val << "\n";
thd.join();

}