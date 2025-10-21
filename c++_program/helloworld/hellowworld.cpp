#include <iostream>
#include <vector>
#include <string>
#include <thread>
using namespace std;

void callable(void)
{
    cout << "thread";
    std::string str;
    cin >> str;

    cout << str;
}


int main()
{
	// Start thread t1
	std::thread t1(callable);

	// Wait for t1 to finish
	t1.join();

	// t1 has finished do other stuff

}
