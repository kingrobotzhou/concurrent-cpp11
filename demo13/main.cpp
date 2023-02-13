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
    /*
    void wait (unique_lock& lck, Predicate pred)为有条件的等待，
    pred是一个可调用的对象或函数，它不接受任何参数，并返回一个可以作为bool计算的值。
    当pred为false时wait()函数才会使线程等待，在收到其他线程通知时只有当pred返回true时才会被唤醒。我们将上述代码做如下修改，它的输出与上述一样。
    */
   
    cv.wait(lck,istrue);
    std::cout << "You entered: " << value << '\n';
 
    th.join();
 
    return 0;
}
