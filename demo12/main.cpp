
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
 
int main()
{
    std::cout << "Please, enter an integer (I'll be printing dots): \n";
    std::thread th(read_value);
 
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);
	/*
	wait()函数因为没有条件判断，因此有时候会产生虚假唤醒，而有条件的等待可以很好的解决这一问题
	*/
    cv.wait(lck);
    std::cout << "You entered: " << value << '\n';
 
    th.join();
 
    return 0;
}