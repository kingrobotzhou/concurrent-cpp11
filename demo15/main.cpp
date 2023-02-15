#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
 
 
using namespace std;
 
static std::mutex mtx;
static bool ready = false;
static std::condition_variable cnd;
static int block_time = 20;


/*
 (1） wait(lock): 调用时即阻塞线程，并且调用lock.unlock() 

（2） wait(lock, conditions): 调用时检查conditions，如果为false,则阻塞线程，并且调用lock.unlock(), 否则，继续执行、

（3） wait_for(lock, time_duration, conditions): 调用时，
     检查条件是否满足:(1) conditions返回true; (2) 时间超时，如果不满足(1)(2)中的一个条件，则阻塞线程，
     并调用lock.unlock(), 否则，到达一定等待时间或满足条件被唤醒 ，注意，等待超过时间段后自动唤醒，
     判断条件一般需要使用者自己在合适的时候判断，并通过notify_one()或notify_all()唤醒，
     所以，使用的时候注意判断返回值，即状态是否为std::cv_status::timeout

（4） wait_until(lock, time_point, conditions): 实际wait_for是通过wait_until实现，
     实际上也是一样的，到达指定时间点或满足条件conditions时被唤醒，注意，到达时间点是自动唤醒，
     判断条件一般需要使用者自己在合适的时候判断，并通过notify_one()或notify_all()唤醒，所以，
     使用的时候注意判断返回值，即状态是否为std::cv_status::timeout

*/
 
void run_thread(int i)
{
    std::cout << __FUNCTION__ << ": run here i = " << i << std::endl;
    std::unique_lock<std::mutex> lg(mtx);
#if 0
    while(!ready)
    {
        std::cout << "run here wait start i = " << i << std::endl;
        cnd.wait(lg);  // 直接阻塞，释放锁
        std::cout << "run here wait i = " << i << std::endl;
    }
#else
    std::cout << __FUNCTION__ << ": run here wait start i = " << i << std::endl;
    cnd.wait(lg, []{ return ready; }); // 如果条件满足，则执行，否则，直接阻塞，释放锁
    std::cout << __FUNCTION__ << ": run here wait i = " << i << std::endl;
#endif
 
    std::cout << __FUNCTION__ << ": run here ready i = " << i << std::endl;
}
 
void run_thread2(int i)
{
    std::cout << __FUNCTION__ << ": run here i = " << i << std::endl;
    std::unique_lock<std::mutex> lg(mtx);
    std::cout << __FUNCTION__ << ": run here wait start i = " << i << ", currentTime = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
    cnd.wait_for(lg, std::chrono::seconds(block_time + (i % 2 == 0 ? 15 : 5)), []{ return ready; }); // 如果条件满足，则执行，否则，直接阻塞，释放锁
    std::cout << __FUNCTION__ << ": run here wait i = " << i << ", ready = " << ready << ", currentTime = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
 
    std::cout << __FUNCTION__ << ": run here ready i = " << i << ", ready = " << ready << ", currentTime = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
}
 
 
void run_thread3(int i)
{
    std::cout << __FUNCTION__ << ": run here i = " << i << std::endl;
    std::unique_lock<std::mutex> lg(mtx);
    std::cout << __FUNCTION__ << ": run here wait start i = " << i << ", currentTime = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
    cnd.wait_until(lg, std::chrono::system_clock::now() + std::chrono::seconds(block_time + (i % 2 == 0 ? 15 : 5)), []{ return ready; }); // 如果条件满足，则执行，否则，直接阻塞，释放锁
    std::cout << __FUNCTION__ << ": run here wait i = " << i << ", ready = " << ready << ", currentTime = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
 
    std::cout << __FUNCTION__ << ": run here ready i = " << i << ", ready = " << ready << ", currentTime = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
}
 
void run_go()
{
    std::unique_lock<std::mutex> lg(mtx);
 
    ready = true;
 
    //    cnd.notify_one();
    cnd.notify_all();
}
 
 
int main()
{
 
 
    {
        std::cout << "================ Case 2 Start ================" << std::endl;
        std::cout << "currentTime A = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
        std::vector<std::thread> threads;
        for(int i = 0; i < 10; ++i)
        {
            threads.push_back(std::thread(run_thread2, i));
        }
        std::cout << "currentTime B = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
        std::cout << "run here create thread end" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(block_time));
        std::cout << "run here create thread wait " << std::endl;
        std::cout << "currentTime C = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
        // run_go();
 
        for(int i = 0; i < 10; ++i)
        {
            if(threads[i].joinable())
            {
                threads[i].join();
            }
        }
        std::cout << "================ Case 2 End ================" << std::endl;
    }
 
    {
        std::cout << "================ Case 3 Start ================" << std::endl;
        std::cout << "currentTime A = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
        std::vector<std::thread> threads;
        for(int i = 0; i < 10; ++i)
        {
            threads.push_back(std::thread(run_thread3, i));
        }
        std::cout << "currentTime B = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
        std::cout << "run here create thread end" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(block_time));
        std::cout << "run here create thread wait " << std::endl;
        std::cout << "currentTime C = " << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() << std::endl;
        //      // run_go();
 
        for(int i = 0; i < 10; ++i)
        {
            if(threads[i].joinable())
            {
                threads[i].join();
            }
        }
        std::cout << "================ Case 3 End ================" << std::endl;
    }
 
    {
        std::cout << "================ Case 1 Start ================" << std::endl;
        std::vector<std::thread> threads;
        for(int i = 0; i < 10; ++i)
        {
            threads.push_back(std::thread(run_thread, i));
        }
        std::cout << "run here create thread end" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(block_time));
        std::cout << "run here create thread wait " << std::endl;
 
        run_go();
 
        for(int i = 0; i < 10; ++i)
        {
            if(threads[i].joinable())
            {
                threads[i].join();
            }
        }
        std::cout << "================ Case 1 End ================" << std::endl;
    }
 
    return 0;
}