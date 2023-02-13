#include <iostream>           
#include <thread>             
#include <mutex>              
#include <condition_variable> 
#include <unistd.h>
 
std::condition_variable cv;
 
int value;
 
void read_value() {
    std::cin >> value;
    cv.notify_one();
}
 
bool istrue()
{
    return value != 0;
}
 
int main()
{
    std::cout << "Please, enter an integer (I'll be printing dots): \n";
    std::thread th(read_value);
 
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck,istrue);
    std::cout << "You entered: " << value << '\n';
 
    th.join();
 
    return 0;
}
