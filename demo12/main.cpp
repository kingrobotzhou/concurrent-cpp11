
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
	（1） wait(lock): 调用时即阻塞线程，并且调用lock.unlock() 

	（2） wait(lock, conditions): 调用时检查conditions，如果为false,则阻塞线程，并且调用lock.unlock(), 否则，继续执行、

	（3） wait_for(lock, time_duration, conditions): 调用时，检查条件是否满足:(1) conditions返回true; (2) 时间超时，如果不满足(1)(2)中的一个条件，则阻塞线程，并调用lock.unlock(), 否则，到达一定等待时间或满足条件被唤醒 ，注意，等待超过时间段后自动唤醒，判断条件一般需要使用者自己在合适的时候判断，并通过notify_one()或notify_all()唤醒，所以，使用的时候注意判断返回值，即状态是否为std::cv_status::timeout

	（4） wait_until(lock, time_point, conditions): 实际wait_for是通过wait_until实现，实际上也是一样的，到达指定时间点或满足条件conditions时被唤醒，注意，到达时间点是自动唤醒，判断条件一般需要使用者自己在合适的时候判断，并通过notify_one()或notify_all()唤醒，所以，使用的时候注意判断返回值，即状态是否为std::cv_status::timeout
	*/

	/*
	wait()函数因为没有条件判断，因此有时候会产生虚假唤醒，而有条件的等待可以很好的解决这一问题
	*/
    cv.wait(lck);
    std::cout << "You entered: " << value << '\n';
 
    th.join();
 
    return 0;
}